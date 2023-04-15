# HeartWave Application

**Authors**: Arda Dolanay, Thomas Wood

## Overview

HeartWave is a Qt-based application designed to monitor and analyze heart rate variability (HRV) to provide insight into a user's coherence levels. The application processes heart rate data collected by a sensor to calculate a coherence score and display it on the screen. The coherence score ranges from 0 to 16 and reflects the user's internal order or harmony, enabling better balance, greater health, and improved creativity, resilience, and productivity.

## Features

1. Real-time heart rate monitoring and HRV analysis
2. Coherence score calculation
3. Coherence level visualization: low (red), medium (blue), and high (green)
4. Challenge levels (1 to 4) to encourage user progress
5. Battery monitoring and low battery alerts
6. Interruption handling due to sensor disconnect or battery issues

## Getting Started

To set up and use the HeartWave application, follow these steps:

1. **Install Qt**: Download and install the Qt framework, making sure the version is Qt 4. Ensure that you have the appropriate version of Qt for your platform and the necessary development tools (such as Qt Creator) installed.
2. **Clone Repository**: Clone the HeartWave application repository to your local machine. If you are using Git, you can use the following command: `git clone https://github.com/ConradBelaire/HRV`.
3. **Open Project**: Launch Qt Creator and open the HeartWave project by selecting "Open Project" from the "File" menu and navigating to the directory where you cloned the repository. Choose the .pro file to open the project.
4. **Configure Build**: Configure the project build settings according to your platform and development environment. This may include selecting the appropriate compiler and setting up the necessary libraries.
5. **Build and Run**: Build the HeartWave application by selecting "Build" from the "Build" menu or by pressing the appropriate keyboard shortcut (e.g., Ctrl+B). Once the build is successful, run the application by selecting "Run" from the "Build" menu or by pressing the appropriate keyboard shortcut (e.g., Ctrl+R).
6. **Monitor**: The sensor will continuously send heart rate data to the HeartWave device, which will then process the data, calculate the coherence score, and update the screen.
7. **Challenge Levels**: Users can select from four challenge levels (1 to 4) to increase the coherence score threshold for low, medium, and high coherence levels. As the user's coherence score improves, they can advance to higher challenge levels.
8. **Battery Management**: The device will monitor the battery level and send a low battery warning to the HeartWave device. Users should recharge or replace the battery as needed.
9. **Session Interruptions**: The application is designed to handle interruptions due to sensor disconnects or battery issues.

## Team Members

Arda Dolanay, Zach Graham, Thomas Wood, and Conrad Belaire

## Team Member Contributions

- **Arda Dolanay**: Heart Rate & Coherence Data, Algorithm to calculate Coherence, Team Project Timeline, and Team Design Implementation Plan, Sequence Diagrams
- **Zach Graham**: DBManager, MainWindow, Log, Session, Menu, Profile, Debugging
- **Thomas Wood**: MainWindow, Log, Session, User Interface Graph Functionality, Debugging, Code Comments, Program Design, Use Cases, Use Case Diagram, UML Class Diagram, Activity Diagrams
- **Conrad Belaire**: User Interface Design & Functionality, MainWindow, Debugging, Traceability Matrix, Demonstration Video

## Organization of the Submission

The submission is organized into 3 folders:

1. **[Project_Requirements](Project_Requirements/)**: This folder contains the documents that outline how the project should have been constructed.
2. **[HRV](HRV/)**: This folder includes the following:
   - *[Design-Documentation](HRV/Design-Documentation/)*: Contains all the diagrams and related documentation.
      - *[Activity_Diagrams](HRV/Design-Documentation/Activity_Diagrams/)*: Activity Diagrams
      - *[Sequence_Diagrams](HRV/Design-Documentation/Sequence_Diagrams/)*: Sequence Diagrams
         - [src](HRV/Design-Documentation/Sequence_Diagrams/src): PlantUML Sequence Diagram Raw Code.
   - *[libs](HRV/libs/)*: Includes external libraries.
   - *[src](HRV/src/)*: Contains the source code for the project.
   - *[ui](HRV/ui/)*: Has the main UI for the project.
3. **[Coherence_Score_Calculator](Coherence_Score_Calculator/)**: This folder contains a separate C++ file that uses an external library to calculate the coherence score. This file does not need to be compiled with the program; it exists on its own.