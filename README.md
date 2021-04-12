# Basketball-Shooting-Robot
Design of Microprocessor Based Systems Project

This final project for an upper-division mechanical design course tasked us with designing and building a PSoC 5LP based device, implementing real-time programming and multitasking in our code and controlling the robot with a LabView VI. Our basketball shooting robot was able to autonomously detect and shoot a ball into a full-size hoop.

I learned a lot from this project:

* Designing finite-state machines and creating state transition diagrams
* Creating map files to select and configure hardware components of a PSoC 5LP using register schematics and peripheral protocols
* Writing and documenting code to collect and interpret UART LiDAR data packets, perform edge detection, and calculate a trajectory
* Real-time programming using interrupts
* Using multitasking to read sensor data and control motors while receiving state transition commands from a LabVIEW GUI via USB
