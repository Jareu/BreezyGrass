import numpy as np
from dataclasses import dataclass
from typing import List, Tuple
import math

@dataclass
class GrassSegment:
    """Represents a single segment of the grass blade"""
    position: np.ndarray  # (x, y) position of segment end
    angle: float         # Angle from vertical in radians
    length: float        # Length of segment
    mass: float         # Mass of segment
    angular_velocity: float = 0.0
    
class GrassBlade:
    def __init__(self, 
                 base_position: Tuple[float, float],
                 num_segments: int = 5,
                 segment_length: float = 0.1,
                 segment_mass: float = 0.01,
                 spring_constant: float = 5.0,
                 damping: float = 0.1):
        """Initialize a blade of grass with connected segments"""
        self.base_position = np.array(base_position, dtype=float)
        self.num_segments = num_segments
        self.spring_constant = spring_constant
        self.damping = damping
        
        # Initialize segments in vertical position
        self.segments: List[GrassSegment] = []
        current_pos = self.base_position.copy()
        
        for i in range(num_segments):
            next_pos = current_pos + np.array([0, segment_length])
            self.segments.append(GrassSegment(
                position=next_pos,
                angle=0.0,  # Start vertical
                length=segment_length,
                mass=segment_mass
            ))
            current_pos = next_pos
    
    def calculate_forces(self, air_pressure: float = 0.0) -> List[float]:
        """Calculate torques on each segment"""
        torques = []
        
        for i, segment in enumerate(self.segments):
            # 1. Gravitational torque
            grav_torque = segment.mass * 9.81 * segment.length * math.sin(segment.angle)
            
            # 2. Spring torque (tries to restore vertical position)
            spring_torque = -self.spring_constant * segment.angle
            
            # 3. Damping torque
            damping_torque = -self.damping * segment.angular_velocity
            
            # 4. Air pressure force
            # Convert pressure to force perpendicular to segment
            pressure_force = air_pressure * segment.length
            pressure_torque = pressure_force * math.cos(segment.angle)
            
            # Sum all torques
            total_torque = grav_torque + spring_torque + damping_torque + pressure_torque
            torques.append(total_torque)
            
        return torques
    
    def update(self, dt: float, air_pressure: float = 0.0):
        """Update the blade's state using simple Euler integration"""
        torques = self.calculate_forces(air_pressure)
        
        for i, (segment, torque) in enumerate(zip(self.segments, torques)):
            # Calculate angular acceleration (τ = I*α)
            moment_of_inertia = (1/3) * segment.mass * segment.length**2
            angular_accel = torque / moment_of_inertia
            
            # Update angular velocity and position
            segment.angular_velocity += angular_accel * dt
            segment.angle += segment.angular_velocity * dt
            
            # Update position based on previous segment
            if i == 0:
                prev_pos = self.base_position
            else:
                prev_pos = self.segments[i-1].position
                
            # Calculate new position
            segment.position = prev_pos + np.array([
                segment.length * math.sin(segment.angle),
                segment.length * math.cos(segment.angle)
            ])