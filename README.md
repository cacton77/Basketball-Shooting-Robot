# Basketball-Shooting-Robot

This final project for an upper-division mechanical design course tasked us with designing and building a PSoC 5LP based device, implementing real-time programming and multitasking in our code and controlling the robot with a LabView VI. Our basketball shooting robot was able to autonomously detect and shoot a ball into a full-size hoop.

I learned a lot from this project:

* Designing finite-state machines and creating state transition diagrams
* Creating map files to select and configure hardware components of a PSoC 5LP using register schematics and peripheral protocols
* Writing and documenting code to collect and interpret UART LiDAR data packets, perform edge detection, and calculate a trajectory
* Real-time programming using interrupts
* Using multitasking to read sensor data and control motors while receiving state transition commands from a LabVIEW GUI via USB

> ![Final Presentation](https://github.com/cacton77/Basketball-Shooting-Robot/blob/main/FinalPresentation.pdf)

> [Spinner Testing](https://youtu.be/g3NIpQnpwTU)

> [LiDAR Testing](https://youtu.be/31K-31oZflY)

> ![State Transition Diagram](https://github.com/cacton77/Basketball-Shooting-Robot/blob/main/StateTransitionDiagram.pdf)

> ![PSoC Code](https://github.com/cacton77/Basketball-Shooting-Robot/blob/main/PSoCCodeFinal.pdf)
