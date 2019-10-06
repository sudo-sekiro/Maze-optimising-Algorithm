# Maze-optimising-Algorithm
Algorithm to autonomously explore and solve any complex, orthogonal, multi-loop maze.
Set up to test using keyboard inputs in the form of "distance left forward right"
where distance is any integer followed by 1's to indicate paths to the left, in front or to the right else 0.
The final point must be a dead end and must be indicated by "distance 2 2 2"
The Algorithm will navigate any maze entered into it evaluating each point using a coordinate system. 
When there are no more unexplored paths it will navigate to the finish.
It will then wait for a keyboard input before navigating from the original starting point to the finish by the path of least distance.
