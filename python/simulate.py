import math
import tkinter as tk
import numpy as np
import threading
import time
from opensimplex import OpenSimplex
import colorsys

class PressureGridSimulation:
    def __init__(self, window_size: int = 640, grid_size: int = 64):
        # Initialize main window
        self.root = tk.Tk()
        self.root.title("Pressure Grid Simulation")
        
        # Constants
        self.WINDOW_SIZE = window_size
        self.GRID_SIZE = grid_size
        self.CELL_SIZE = window_size // grid_size
        self.TARGET_FPS = 30
        self.simulation_speed = 1.0
        self.running = True
        self.pause_on_next_step = False
        self.paused = False
        
        self.reflecting_boundaries = False  # Can be toggled
        self.boundary_damping = 0.95      # Damping factor for absorbing boundaries
        
        # Initialize noise generator with random seed
        self.noise_gen = OpenSimplex(seed=int(time.time()))
        
        # Initialize pressure grid with simplex noise
        self.pressure_grid = self._generate_noise_grid()
        
        # Setup canvas
        self.canvas = tk.Canvas(
            self.root,
            width=window_size,
            height=window_size,
            background='black'
        )
        self.canvas.pack()
        
        # Initialize grid cells
        self.grid_cells = [[0 for _ in range(grid_size)] for _ in range(grid_size)]
        self._create_grid()
        
        # Physics constants
        self.DIFFUSION_RATE = 0.005  # How quickly pressure spreads
        self.DAMPING = 0.99        # Energy loss in the system
        self.DT = 1/self.TARGET_FPS           # Time step (matches our frame rate)
        
        # Add velocity grid to track fluid movement
        self.velocity_x = np.zeros((grid_size, grid_size), dtype=float)
        self.velocity_y = np.zeros((grid_size, grid_size), dtype=float)
        
        # Start simulation thread
        self.sim_thread = threading.Thread(target=self._simulation_loop, daemon=True)
        self.sim_thread.start()
        
        self._setup_interactions()
    
    def _generate_noise_grid(self, scale: float = 3.0) -> np.ndarray:
        """Generate a grid of simplex noise values"""
        grid = np.zeros((self.GRID_SIZE, self.GRID_SIZE), dtype=float)
        
        for i in range(self.GRID_SIZE):
            for j in range(self.GRID_SIZE):
                # Scale coordinates to get more interesting noise patterns
                x = i / self.GRID_SIZE * scale
                y = j / self.GRID_SIZE * scale
                # Generate noise value between -1 and 1
                grid[i, j] = self.noise_gen.noise2(x, y)
        
        return grid
    
    def _pressure_to_color(self, pressure: float) -> str:
        """Convert pressure value to color string"""
        # Handle NaN pressure values
        if np.isnan(pressure) or pressure == float('inf'):
            return '#000000'
        
        # Clamp pressure between -1 and 1
        pressure = max(-1.0, min(1.0, pressure))

        # Normalize pressure to [0, 1] range (pressure is already in [-1, 1])
        normalized = (pressure + 1) / 2
        
        # Convert HSV to RGB (using blue->red spectrum)
        hue = (1 - normalized) * 0.7  # 0.7 = blue, 0 = red
        rgb = colorsys.hsv_to_rgb(hue, 1.0, 1.0)
        
        # Convert to hex color string
        return f'#{int(rgb[0]*255):02x}{int(rgb[1]*255):02x}{int(rgb[2]*255):02x}'
    
    def _create_grid(self):
        """Create initial grid of rectangles on canvas"""
        for i in range(self.GRID_SIZE):
            for j in range(self.GRID_SIZE):
                x1 = j * self.CELL_SIZE
                y1 = i * self.CELL_SIZE
                x2 = x1 + self.CELL_SIZE
                y2 = y1 + self.CELL_SIZE
                
                # Create rectangle and store its ID
                self.grid_cells[i][j] = self.canvas.create_rectangle(
                    x1, y1, x2, y2,
                    fill='black',
                    outline='darkgrey'
                )
    
    def _update_grid_display(self):
        """Update the visual representation of the grid"""
        for i in range(self.GRID_SIZE):
            for j in range(self.GRID_SIZE):
                color = self._pressure_to_color(self.pressure_grid[i][j])
                self.canvas.itemconfig(self.grid_cells[i][j], fill=color)
    
    def _simulate_pressure_step(self):
        """Simulate one step of pressure wave propagation"""

        dx = 1.0 / self.GRID_SIZE
        
        # Calculate pressure gradients
        pressure_grad_x = np.zeros_like(self.pressure_grid)
        pressure_grad_y = np.zeros_like(self.pressure_grid)
        
        # Central difference for pressure gradients
        pressure_grad_x[:, 1:-1] = (self.pressure_grid[:, 2:] - self.pressure_grid[:, :-2]) / (2 * dx)
        pressure_grad_y[1:-1, :] = (self.pressure_grid[2:, :] - self.pressure_grid[:-2, :]) / (2 * dx)
        
        # Update velocities based on pressure gradients
        self.velocity_x -= self.DT * pressure_grad_x
        self.velocity_y -= self.DT * pressure_grad_y
        
        # Apply damping to velocities
        self.velocity_x *= self.DAMPING
        self.velocity_y *= self.DAMPING
        
        # Update pressure based on velocity divergence
        divergence = np.zeros_like(self.pressure_grid)
        divergence[1:-1, 1:-1] = (
            (self.velocity_x[1:-1, 2:] - self.velocity_x[1:-1, :-2]) / (2 * dx) +
            (self.velocity_y[2:, 1:-1] - self.velocity_y[:-2, 1:-1]) / (2 * dx)
        )
        
        # Update pressure using the wave equation
        self.pressure_grid -= self.DIFFUSION_RATE * divergence
        
        if self.reflecting_boundaries:
            # Reflecting boundaries (existing behavior)
            self.pressure_grid[0, :] = self.pressure_grid[1, :]
            self.pressure_grid[-1, :] = self.pressure_grid[-2, :]
            self.pressure_grid[:, 0] = self.pressure_grid[:, 1]
            self.pressure_grid[:, -1] = self.pressure_grid[:, -2]
        else:
            # Absorbing boundaries (damping zone)
            border_width = 3  # Width of damping zone
            
            # Apply damping to border regions
            for i in range(border_width):
                damping = self.boundary_damping * (i + 1) / border_width
                
                # Top and bottom borders
                self.pressure_grid[i, :] *= damping
                self.pressure_grid[-(i+1), :] *= damping
                
                # Left and right borders
                self.pressure_grid[:, i] *= damping
                self.pressure_grid[:, -(i+1)] *= damping
                
                # Also damp velocities in border regions
                self.velocity_x[i, :] *= damping
                self.velocity_x[-(i+1), :] *= damping
                self.velocity_x[:, i] *= damping
                self.velocity_x[:, -(i+1)] *= damping
                
                self.velocity_y[i, :] *= damping
                self.velocity_y[-(i+1), :] *= damping
                self.velocity_y[:, i] *= damping
                self.velocity_y[:, -(i+1)] *= damping

    def _simulation_loop(self):
        """Main simulation loop running in separate thread"""
        while self.running:
            loop_start = time.time()
            
            # Simulate pressure waves
            if not self.paused:
                self._simulate_pressure_step()
                self.root.after(0, self._update_grid_display)
            
            if self.pause_on_next_step:
                self.paused = True
                self.pause_on_next_step = False
            
            # Maintain target FPS
            elapsed = time.time() - loop_start
            sleep_time = max(0, (1.0/self.TARGET_FPS) - elapsed)
            time.sleep(sleep_time / self.simulation_speed)
    
    def run(self):
        """Start the application"""
        try:
            self.root.mainloop()
        finally:
            self.running = False
            self.sim_thread.join()

    def _simulate_pressure(self):
        """Simulate pressure on the grid"""

    def _set_pressure(self, x: int, y: int, pressure: float):
        """Set the pressure at the specified grid location"""
        if 0 <= x < self.GRID_SIZE and 0 <= y < self.GRID_SIZE:
            self.pressure_grid[y, x] = pressure

    def _setup_interactions(self):
        """Setup mouse interactions"""
        def on_click(event):
            self._handle_click(event, 1.5)

        def shift_click(event):
            self._handle_click(event, 0.0)
        
        self.canvas.bind("<Button-1>", on_click)
        self.canvas.bind("<Shift-Button-1>", shift_click)
    
    def _handle_click(self, event, pressure: float):
        """Handle regular mouse clicks"""
        circle_radius = 10
        ring_thickness = 4
        cursor_x = int(event.x / self.CELL_SIZE)
        cursor_y = int(event.y / self.CELL_SIZE)

        if 0 <= cursor_x < self.GRID_SIZE and 0 <= cursor_y < self.GRID_SIZE: 
            for i in range(-circle_radius, circle_radius + 1):
                for j in range(-circle_radius, circle_radius + 1):
                    distance = math.sqrt(i**2 + j**2)
                    if distance <= circle_radius:
                        self._set_pressure(cursor_x + i, cursor_y + j, pressure)
    
if __name__ == "__main__":
    sim = PressureGridSimulation()
    sim.run()