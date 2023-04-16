HeartWave Application

Author: Arda Dolanay

Overview

HeartWave is a Qt-based application designed to monitor and analyze heart rate variability (HRV) to provide insight into a user's coherence levels. The application processes heart rate data collected by a sensor to calculate a coherence score and display it on the screen. The coherence score ranges from 0 to 16 and reflects the user's internal order or harmony, enabling better balance, greater health, and improved creativity, resilience, and productivity.


Features

1. Real-time heart rate monitoring and HRV analysis
2. Coherence score calculation
3. Coherence level visualization: low (red), medium (blue), and high (green)
4. Challenge levels (1 to 4) to encourage user progress
5. Battery monitoring and low battery alerts
6. Interruption handling due to sensor disconnect or battery issues

Getting Started

To set up and use the HeartWave application, follow these steps:
1. Install Qt: Download and install the Qt framework, make sure the version is QT 4. Ensure that you have the appropriate version of Qt for your platform and the necessary development tools (such as Qt Creator) installed.
2. Clone Repository: Clone the HeartWave application repository to your local machine. If you are using Git, you can use the following command: git clone https://github.com/ConradBelaire/HRV.
3. Open Project: Launch Qt Creator and open the HeartWave project by selecting "Open Project" from the "File" menu and navigating to the directory where you cloned the repository. Choose the .pro file to open the project.
4. Configure Build: Configure the project build settings according to your platform and development environment. This may include selecting the appropriate compiler and setting up the necessary libraries.


5. Build and Run: Build the HeartWave application by selecting "Build" from the "Build" menu or by pressing the appropriate keyboard shortcut (e.g., Ctrl+B). Once the build is successful, run the application by selecting "Run" from the "Build" menu or by pressing the appropriate keyboard shortcut (e.g., Ctrl+R).
6. Monitor: The Sensor will continuously send heart rate data to the HeartWave Device, which will then process the data, calculate the coherence score, and update the Screen.
7. Challenge Levels: Users can select from four challenge levels (1 to 4) to increase the coherence score threshold for low, medium, and high coherence levels. As the user's coherence score improves, they can advance to higher challenge levels.
8. Battery Management: The Sensor will monitor the battery level and send a low battery warning to the HeartWave Device, which will then display an alert on the Screen. Users should recharge or replace the battery as needed.


9. Session Interruptions: The application is designed to handle interruptions due to sensor disconnects or battery issues. Once the issue is resolved, the HeartWave Device will resume processing heart rate data and updating the Screen.


Team Members

Arda Dolanay, Zach Graham, Thomas Wood and Conrad Belaire




Team Member Contributions

Arda Dolanay: Sequence Diagram. Coherence Score Calculator. Created the coherence algorithm that corresponds to the specific dataset of heart rates. Project Timeline, and Design Implementation Plan.

Zach Graham: Implemented the whole database for the program. A part of the logical interpitations of the program, including class creations, functions, variable allocation and such.

Thomas Wood: UML Diagram. Use Case Diagram. Action Diagram. A part of the Documentation of the program. A part of the logical interpitations of the program, including class creations, functions, variable alocation and such. A part of the design implementation regarding the UI.

Conrad Belaire: Traceibility Matrix. A part of the design implementation regarding the UI. A part of the documentation of the program. A part of the logical interpitations of the program, including class creations, functions, variable alocation and such.



Organization of The Submission

The sunmission is organized into 3 Folders, Project_Requirements, HRV, and the Coherence Score Calculator which has a sperate c++ file that uses an external library to calculate the coherence score. This file does not need to be compiled with the program, it just exists on its own. The HRV folder has with it the Design-Documentation, which includes all the diagrams and such, libs which includes external libraries, scr which has the source code for the project, ui which has the main ui for the project. Lastly the Project_Requirements folders has the documents that outline how the project should have been constructed.
