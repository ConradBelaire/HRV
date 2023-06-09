#include "mainwindow.h"
#include "ui_mainwindow.h"


/**
 * @brief MainWindow constructor: Initializes the main window of the application, including the user interface (UI), settings,
 * and other necessary components.
 *
 * @param parent: The parent widget for the main window.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // init ui
    ui->setupUi(this);

    // init settings
    pacer_dur = 10;
    challenge_level = 1;
    pacerCounter = -1;

    // Set initial Skin status
    connectedStatus = false;

    // Initialize the timer
    currentTimerCount = -1;

    // create database dbmanager
    dbmanager = new DBManager();

    // create profile
    profile = dbmanager->getProfile(0);

    // Initialize the menu
    masterMenu = new Menu("MAIN MENU", {"BEGIN SESSION","HISTORY","SETTINGS"}, nullptr);
    mainMenuOG = masterMenu;
    initializeMainMenu(masterMenu);

    // start program with device in an off state
    powerStatus = false;
    changePowerStatus();

    // connect user widgets
    connect(ui->batteryLevelAdminSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::changeBatteryLevel);
    connect(ui->powerButton, &QPushButton::released, this, &MainWindow::powerChange);
    connect(ui->chargeBatteryButton, &QPushButton::released, this, &MainWindow::rechargeBattery);
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
    connect(ui->okButton, &QPushButton::pressed, this, &MainWindow::navigateSubMenu);
    connect(ui->menuButton, &QPushButton::pressed, this, &MainWindow::navigateToMainMenu);
    connect(ui->backButton, &QPushButton::pressed, this, &MainWindow::navigateBack);
    connect(ui->skinToggle, &QPushButton::pressed, this, &MainWindow::toggleSkin);
    connect(ui->dropTable, &QPushButton::pressed, this, &MainWindow::dropTables);
    connect(ui->redButton, &QPushButton::pressed, this, &MainWindow::toggleRedLED);
    connect(ui->greenButton, &QPushButton::pressed, this, &MainWindow::toggleGreenLED);
    connect(ui->blueButton, &QPushButton::pressed, this, &MainWindow::toggleBlueLED);

    // Initialize battery levels
    ui->batteryLevelAdminSpinBox->setValue(profile->getBLvl());

    // setup for lights
    redOn = "background-color: rgb(220, 0, 0)";
    redOff = "background-color: rgb(80, 0, 0)";
    greenOn = "background-color: rgb(0, 170, 0)";
    greenOff = "background-color: rgb(0, 50, 0)";
    blueOn = "background-color: rgb(0, 0, 230)";
    blueOff = "background-color: rgb(0, 0, 80)";
    connectionOn = "background-color: rgb(255, 0, 142)";
    connectionOff = "background-color: rgb(142, 0, 142)";
    turnOffLights();

    // randomize for heart vector selection
    std::srand(static_cast<unsigned>(std::time(0)));

    // setup graphs
    // session graph
    ui->sessionFrame->setVisible(false);
    ui->summaryFrame->setVisible(false);
    maxHR = 100;
    minHR = 60;
    ui->customPlot->xAxis->setRange(0, 5);
    ui->customPlot->yAxis->setRange(minHR, maxHR);
    ui->customPlot->xAxis->setLabel("Time (s)");
    ui->customPlot->yAxis->setLabel("HR");
    ui->customPlot->addGraph(); // active session graph
    ui->customPlot->graph(0)->setPen(QPen(Qt::red)); // set the pen color
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine); // set the line style
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssNone);
    // summary graph
    ui->customPlot_2->xAxis->setLabel("Time (s)");
    ui->customPlot_2->yAxis->setLabel("HR");
    ui->customPlot_2->addGraph(); // active session graph
    ui->customPlot_2->graph(0)->setPen(QPen(Qt::red)); // set the pen color
    ui->customPlot_2->graph(0)->setLineStyle(QCPGraph::lsLine); // set the line style
    ui->customPlot_2->graph(0)->setScatterStyle(QCPScatterStyle::ssNone);

    // session / summary visiblity setup
    sessionSummaryVisible = false;
    inSessionView = false;
    startSession = false;

    // hardcoded hr data
    vectorHRcount = 10; // max 9. we set it to 10 so it picks an array of first time call
    heartRateData = {
        {60, 60, 60, 60, 60, 60, 60, 60, 60, 60}, // Coherence Score: 0.4 | Challenge Level: 1
        {80, 80, 80, 80, 80, 80, 80, 80, 80, 80}, // Coherence Score: 2.5 | Challenge Level: 2
        {90, 90, 90, 90, 90, 90, 90, 90, 90, 90}, // Coherence Score: 4.5 | Challenge Level: 3

        {70, 75, 72, 78, 71, 68, 74, 73, 69, 71}, // Coherence Score: 0.7 | Challenge Level: 1
        {65, 70, 65, 75, 68, 67, 66, 68, 69, 70}, // Coherence Score: 1.3 | Challenge Level: 1
        {76, 78, 79, 77, 76, 75, 78, 79, 78, 77}, // Coherence Score: 1.8 | Challenge Level: 2
        {85, 83, 84, 85, 86, 87, 84, 85, 83, 82}, // Coherence Score: 3.5 | Challenge Level: 3
        {78, 78, 79, 79, 78, 77, 77, 76, 76, 76}, // Coherence Score: 2.9 | Challenge Level: 2
        {90, 92, 91, 89, 88, 90, 91, 92, 90, 89}, // Coherence Score: 4.2 | Challenge Level: 3
        {70, 71, 69, 68, 70, 71, 72, 74, 75, 76}, // Coherence Score: 1.5 | Challenge Level: 1
        {82, 81, 80, 82, 83, 84, 85, 84, 83, 82}, // Coherence Score: 6.0 | Challenge Level: 4
        {75, 76, 74, 73, 72, 71, 72, 73, 74, 75}, // Coherence Score: 1.2 | Challenge Level: 1
        {88, 86, 87, 85, 83, 84, 85, 86, 87, 88}, // Coherence Score: 3.8 | Challenge Level: 3
        {67, 68, 69, 70, 71, 72, 73, 74, 75, 76}, // Coherence Score: 2.2 | Challenge Level: 2
        {80, 79, 78, 77, 76, 75, 74, 73, 72, 71}, // Coherence Score: 1.9 | Challenge Level: 1
        {95, 93, 94, 95, 96, 95, 94, 93, 92, 91}, // Coherence Score: 5.5 | Challenge Level: 4
        {77, 77, 78, 79, 79, 78, 78, 77, 76, 76}, // Coherence Score: 2.7 | Challenge Level: 2
        {92, 91, 90, 89, 88, 87, 86, 85, 84, 83}, // Coherence Score: 4.0 | Challenge Level: 3
        {73, 73, 72, 72, 71, 71, 70, 70, 69, 69}, // Coherence Score: 0.9 | Challenge Level: 1
        {81, 80, 81, 82, 83, 84, 85, 86, 87, 88}, // Coherence Score: 6.2 | Challenge Level: 4
        {74, 75, 76, 77, 78, 79, 80, 81, 82, 83}, // Coherence Score: 1.4 | Challenge Level: 1
        {89, 88, 87, 86, 85, 84, 83, 82, 81, 80}, // Coherence Score: 3.3 | Challenge Level: 3
        {84, 85, 86, 87, 88, 89, 90, 91, 92, 93}, // Coherence Score: 2.8 | Challenge Level: 2
        {70, 71, 72, 73, 74, 75, 76, 77, 78, 79}, // Coherence Score: 0.6 | Challenge Level: 1
        {97, 96, 95, 94, 93, 92, 91, 90, 89, 88}, // Coherence Score: 5.2 | Challenge Level: 4
        {72, 73, 74, 75, 76, 77, 78, 79, 80, 81}, // Coherence Score: 1.6 | Challenge Level: 1
        {87, 86, 85, 84, 83, 82, 81, 80, 79, 78}, // Coherence Score: 3.5 | Challenge Level: 3
        {82, 83, 84, 85, 86, 87, 88, 89, 90, 91}, // Coherence Score: 2.4 | Challenge Level: 2
        {69, 70, 71, 72, 73, 74, 75, 76, 77, 78}, // Coherence Score: 0.8 | Challenge Level: 1
    };
}

/**
 * @brief MainWindow destructor: Deletes and cleans up the dynamically allocated objects and memory associated with the main window.
 */
MainWindow::~MainWindow() {
    dbmanager->addProfile(profile->getId(), profile->getBLvl(), profile->getSessAmt());

    delete mainMenuOG;
    delete ui;
    for (int i = 0; i < sessions.size(); i++) {
        delete sessions[i];
    }
    delete dbmanager;
    delete profile;
    delete activeQListWidget;
    // if (timer != nullptr) {delete timer;}
    delete currentSession;
}

/**
 * @brief Creates the history menu, including session list, clear history option, and child menus for each session.
 * @param m Pointer to the parent menu of the history menu.
 * @return Pointer to the created history menu.
 */
Menu* MainWindow::create_history_menu(Menu* m) {
        // initialise session list
        QStringList sessionList;
        sessionList.append("CLEAR");

        QVector<Log*>* logs = dbmanager->getLogs();

        for (Log* currentLog : *logs) {
            sessionList.append("Session Number: " + QString::number(currentLog->getId()));
        }

        //create parent menu
        Menu* history = new Menu("HISTORY", sessionList, m);

        // create child menus
        Menu* clearHistory = new Menu("CLEAR", {"YES","NO"}, history);
        history->addChildMenu(clearHistory);
        sessions = QVector<Session*>();

        for (Log* currentLog : *logs) {


            Menu* session_menu = new Menu(QString::number(currentLog->getId()), {"VIEW", "DELETE"}, history);
            sessions.append(new Session(currentLog));

            Menu* view = new Menu("VIEW", {QString::number(currentLog->getId())}, session_menu);
            Menu* delete_menu = new Menu("DELETE", {"YES","NO"}, session_menu);
            session_menu->addChildMenu(view);
            session_menu->addChildMenu(delete_menu);

            history->addChildMenu(session_menu);
        }
        return history;
}

/**
 * @brief Creates the settings menu, including options for resetting, changing challenge level, and setting pacer duration.
 * @param m Pointer to the parent menu of the settings menu.
 * @return Pointer to the created settings menu.
 */
Menu* MainWindow::create_settings_menu(Menu* m){
    Menu* settings = new Menu("SETTINGS", {"RESET", "CHALLENGE LEVEL", "PACER DURATION"}, m);
    Menu* reset = new Menu("RESET", {"YES","NO"}, settings);
    Menu* challengeLevel = new Menu("CHALLENGE LEVEL", {"1","2","3","4"}, settings);
    Menu* pacerDuration = new Menu(
        "PACER DURATION",
        {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30"},
        settings
    );
    settings->addChildMenu(reset);
    settings->addChildMenu(challengeLevel);
    settings->addChildMenu(pacerDuration);
    return settings;
}

/**
 * @brief Initializes the main menu by adding menu items to the active QListWidget, setting the menu label, and creating
 * sub-menus for begin session, history, and settings.
 * @param m Pointer to the main menu object to be initialized.
 */
void MainWindow::initializeMainMenu(Menu* m) {
    activeQListWidget = ui->mainMenuListView;
    activeQListWidget->addItems(masterMenu->getMenuItems());
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(masterMenu->getName());

    // create begin_session menu
    Menu* begin_session = new Menu("BEGIN SESSION", {}, m);
    m->addChildMenu(begin_session);

    //create history menu
    Menu* history = create_history_menu(m);
    m->addChildMenu(history);

    // create settings menu
    Menu* settings = create_settings_menu(m);
    m->addChildMenu(settings);
}

/**
 * Navigates the selection up within the active QListWidget.
 */
void MainWindow::navigateUpMenu() {

    int nextIndex = activeQListWidget->currentRow() - 1;

    if (nextIndex < 0) {
        nextIndex = activeQListWidget->count() - 1;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}

/**
 * Navigates the selection down within the active QListWidget.
 */
void MainWindow::navigateDownMenu() {

    int nextIndex = activeQListWidget->currentRow() + 1;

    if (nextIndex > activeQListWidget->count() - 1) {
        nextIndex = 0;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}

/**
 * @brief Navigates to the main menu. If a session is in progress, displays the session summary. If the session summary is visible,
 * clears the session summary. This function creates a new main menu and initializes it, then updates the display with
 * the new main menu's name and items.
 */
void MainWindow::navigateToMainMenu() {

    if (currentTimerCount != -1) {
        displaySummary(currentSession, false);
    } else if (sessionSummaryVisible) {
        clearSessionSummary();
    }

    // go to main menu
    masterMenu = new Menu("MAIN MENU", {"BEGIN SESSION","HISTORY","SETTINGS"}, nullptr);
    initializeMainMenu(masterMenu);
    updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
}

/**
 * @brief Navigates back through the menu hierarchy based on the current state. If a session is in progress or the session summary
 * is visible, these will be displayed or cleared respectively. If the user is in the session view, it will exit the view
 * and return to the main menu.
 */
void MainWindow::navigateBack() {
    if (currentTimerCount > 0) {
        displaySummary(currentSession, false);
        return;
    } else if (sessionSummaryVisible) {
        clearSessionSummary();
        return;
    } else if (inSessionView) {
        inSessionView = false;
        startSession = false;
        currentTimerCount = -1;
        pacerCounter = -1;
        pacerCountDown = false;
        pacerWait = false;
        pacerCountUp = true;
        ui->sessionFrame->setVisible(false);
        navigateToMainMenu();
        return;
    }

    if(masterMenu->getName() == "BEGIN SESSION") {
        masterMenu = new Menu("MAIN MENU", {"BEGIN SESSION","HISTORY","SETTINGS"}, nullptr);
        initializeMainMenu(masterMenu);
        updateMenu(mainMenuOG->getName(), mainMenuOG->getMenuItems());
        return;
    }

    if(masterMenu->getName() == "CLEAR") {
        masterMenu = new Menu("MAIN MENU", {"BEGIN SESSION","HISTORY","SETTINGS"}, nullptr);
        initializeMainMenu(masterMenu);
        updateMenu(mainMenuOG->getName(), mainMenuOG->getMenuItems());
        return;
    }

    if (masterMenu->getName() == "MAIN MENU") {
        activeQListWidget->setCurrentRow(0);
    }

    if (masterMenu->getParent() == nullptr) {
        masterMenu = new Menu("MAIN MENU", {"BEGIN SESSION","HISTORY","SETTINGS"}, nullptr);
        initializeMainMenu(masterMenu);
        updateMenu(mainMenuOG->getName(), mainMenuOG->getMenuItems());
        return;
    }

    if(masterMenu->getParent()->getName() == "HISTORY"){
        navigateToMainMenu();
        activeQListWidget->setCurrentRow(1);
        navigateSubMenu();
    } else {
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }
}

/**
 * @brief Handles navigation within submenus, as well as the functionality for each submenu item. This function includes logic
 * for menu items such as starting a session, resetting logs, changing settings, and more. It checks the name of the
 * current menu and the selected menu item to determine the appropriate action to take.
 */
void MainWindow::navigateSubMenu() {
    int index = activeQListWidget->currentRow();

    if (inSessionView) {
        if (!startSession) {
            startSession = true;;
        } else if (currentTimerCount > 0) {
            startSession = false;
        }
        applyToSkin(connectedStatus);
    }


    if (index < 0) return;


    // fucntionality of the clear menu
    if (masterMenu->getName() == "CLEAR"){
        if (masterMenu->getMenuItems().value(index) == "YES") {
            dbmanager->deleteLogs();
            navigateBack();
            return;
        }
        else {
            navigateBack();
            return;
        }
    }

    // fucntionality of the delete menu
    if(masterMenu->getName() == "DELETE"){
        if (masterMenu->getMenuItems().value(index) == "YES") {
            int log_id = masterMenu->getParent()->getParent()->getName().toInt() -1;
            dbmanager->deleteLog(log_id);
            navigateBack();
            return;
        }
        else {
            navigateBack();
            return;
        }

        // this isnt great
    }

    // fucntionality of the reset menu
    if(masterMenu->getName() == "RESET"){
        if (masterMenu->getMenuItems().value(index) == "YES") {
            dbmanager->deleteLogs();
            challenge_level = 1;
            pacer_dur = 10;
            navigateBack();
            powerChange();
            return;
        }
        else {
            navigateBack();
            return;
        }
    }

    // fucntionality of the challenge menu
    if(masterMenu->getName() == "CHALLENGE LEVEL"){
        challenge_level = index + 1;
        navigateBack();
        return;
    }

    // navigate to Pacer duration menu
    if(masterMenu->getName() == "PACER DURATION"){
        pacer_dur = index + 1;
        navigateBack();
        return;
    }

    // navigate to begin_session menu
    if(masterMenu->getChildMenu(index)->getName() == "BEGIN SESSION") {
        masterMenu = masterMenu->getChildMenu(index);
        MainWindow::updateMenu(masterMenu->getName(), {});
        MainWindow::start_session();
        inSessionView = true;
        return;
    }

    // navigate to history menu
    if (masterMenu->getChildMenu(index)->getName() == "HISTORY") {
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }

    // navigate into the view menu
    if (masterMenu->getChildMenu(index)->getName() == "VIEW") {
        displaySummary(sessions[masterMenu->getName().toInt() - 1], true);
        return;
    }


    // navigate into the delete menu
    if (masterMenu->getChildMenu(index)->getName() == "DELETE") {
        dbmanager->deleteLog(masterMenu->getName().toInt());
        navigateBack();
        return;
    }

    // navigate to clear menu
    if (masterMenu->getChildMenu(index)->getName() == "CLEAR") {
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }


    // navigate to session number menu
    if (masterMenu->getMenuItem(index) == "Session Number: "+QString::number(index)) {
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }

    // navigate to settings menu
    if (masterMenu->getChildMenu(index)->getName() == "SETTINGS") {
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }

    // navigate to Reset menu
    if (masterMenu->getChildMenu(index)->getName() == "RESET") {
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }


    // navigate to challenge menu
    if (masterMenu->getChildMenu(index)->getName() == "CHALLENGE LEVEL") {
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }

    // navigate to Pacer duration menu
    if (masterMenu->getChildMenu(index)->getName() == "PACER DURATION") {
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }
}

/**
 * @brief Updates the current active QListWidget menu with a new set of menu items and a new title. This function is used to
 * refresh the menu UI when navigating between menus.
 *
 * @param selectedMenuItem The QString for the new title of the menu to be displayed.
 * @param menuItems The QStringList containing the new set of menu items to be displayed in the QListWidget.
 */
void MainWindow::updateMenu(const QString &selectedMenuItem, const QStringList &menuItems) {

    activeQListWidget->clear();
    activeQListWidget->addItems(menuItems);
    activeQListWidget->setCurrentRow(0);

    ui->menuLabel->setText(selectedMenuItem);
}

/**
 * Recharges the battery to its full capacity (100%). This function is used to simulate the
 * battery being recharged to its maximum level.
 */
void MainWindow::rechargeBattery(){
    changeBatteryLevel(100);
}

/**
 * @brief Changes the battery level and updates the UI accordingly.
 *
 * @param newLevel A double value representing the new battery level (0.0 to 100.0).
 */
void MainWindow::changeBatteryLevel(double newLevel) {

    if (newLevel >= 0.0 && newLevel <= 100.0) {
        if (newLevel == 0.0 && powerStatus == true) {
            powerChange();
            profile->setBLvl(0);
        }else{
            profile->setBLvl(newLevel);
        }

        ui->batteryLevelAdminSpinBox->setValue(newLevel);
        int newLevelInt = int(newLevel);
        ui->batteryLevelBar->setValue(newLevelInt);

        // Sets 3 different CSS stylings for green, yellow, red battery levels
        QString highBatteryHealth = "QProgressBar { selection-background-color: rgb(78, 154, 6); background-color: rgb(0, 0, 0); }";
        QString mediumBatteryHealth = "QProgressBar { selection-background-color: rgb(196, 160, 0); background-color: rgb(0, 0, 0); }";
        QString lowBatteryHealth = "QProgressBar { selection-background-color: rgb(164, 0, 0); background-color: rgb(0, 0, 0); }";

        if (newLevelInt >= 50) {
            ui->batteryLevelBar->setStyleSheet(highBatteryHealth);
        }
        else if (newLevelInt >= 20) {
            ui->batteryLevelBar->setStyleSheet(mediumBatteryHealth);
        }
        else {
            ui->batteryLevelBar->setStyleSheet(lowBatteryHealth);
        }
    }
}

/**
 * @brief Toggles the power status of the MainWindow, will save session if one is active
 */
void MainWindow::powerChange(){

    // if in the middle of a session
    if (currentTimerCount > 0){
        //Save Session
        applyToSkin(false);
        displaySummary(currentSession, false);
        clearSessionSummary();
    }

    // if the battery level is greater than 0, then toggle the power status
    if (profile->getBLvl() > 0) {
        powerStatus  = !powerStatus;
        changePowerStatus();
    }

    ui->sessionFrame->setVisible(false);
    ui->summaryFrame->setVisible(false);
}

/**
 * @brief Updates the power status of the MainWindow, managing the UI elements
 *        visibility and state according to the current power status.
 */
void MainWindow::changePowerStatus() {
    if (!powerStatus) {turnOffLights();}
    dbmanager->updateProfile(profile->getId(), profile->getBLvl());

    ui->screen->setVisible(powerStatus); // Sets the whole screen widget's and all children's visibility

    if (powerStatus) {
        if (connectedStatus) {ui->hrConnection->setStyleSheet(connectionOn);}
        MainWindow::navigateToMainMenu();
    }

    ui->upButton->setEnabled(powerStatus);
    ui->downButton->setEnabled(powerStatus);
    ui->leftButton->setEnabled(powerStatus);
    ui->rightButton->setEnabled(powerStatus);
    ui->menuButton->setEnabled(powerStatus);
    ui->okButton->setEnabled(powerStatus);
    ui->backButton->setEnabled(powerStatus);
}

/**
 * @brief Initializes and a new session, creating a new Session object,
 *        updating the UI elements, and initializing the session timer.
 */
void MainWindow::start_session(){
    //make sesions ui visible
    ui->pacerBar->setValue(0);
    ui->sessionFrame->setVisible(true);


    currentTimerCount = 0;
    pacerCounter = 0;
    pacerCountUp = true;
    pacerWait = false;
    pacerCountDown = false;

    // initialize the timer
    timer = new QTimer(this);
    init_timer(timer);

    // create session
    int thisSessionID = profile->increaseSessAmt();
    currentSession = new Session(thisSessionID, challenge_level, pacer_dur, timer);
}

/**
 * @brief Initializes a timer object and connects its timeout signal to the update_timer slot.
 *
 * @param timer The QTimer object to initialize and connect.
 */
void MainWindow::init_timer(QTimer* timer){
    connect(timer, &QTimer::timeout, this, &MainWindow::update_timer);

    if (connectedStatus && startSession){
        timer->start(1000);
    }
}

/**
 * @brief Updates the session timer, heart rate data, coherence score, UI elements,
 *        and LED lights based on the current session state.
 *
 * 1. Drains the battery at each timer update.
 * 2. Updates the duration text displayed on the UI.
 * 3. Generates a new heart rate value and updates the y-axis of the plot accordingly.
 * 4. Updates the x-axis of the plot if necessary to show a sliding window of the last 5 seconds.
 * 5. Adds the new heart rate data to the plot and re-draws the plot.
 * 6. Calculates the new coherence score and updates the achievement score.
 * 7. Updates the achievement score and coherence score text displayed on the UI.
 * 8. Determines which LED light to turn on based on the coherence score.
 * 9. Updates the pacer based on the current session state.
 */
void MainWindow::update_timer(){
    drainBattery();

    // update duration text
    ui->lengthBar->setText(QString::number(currentTimerCount) + "s");

    currentTimerCount++;
    int newHeartRate = generateHR();   // some function should be here to set this value. the function could look up an array heart rates based of currentTimerCount

    // update y axis
    if (newHeartRate < minHR) {
        minHR = newHeartRate;
    } else if (newHeartRate > maxHR) {
        maxHR = newHeartRate;
    }
    ui->customPlot->yAxis->setRange(minHR, maxHR);

    // should we update x axis?
    if (currentTimerCount > 5) {
        ui->customPlot->xAxis->setRange(currentTimerCount-5, 1+currentTimerCount);
    }

    // add in data and replot
    ui->customPlot->graph(0)->addData(currentTimerCount, newHeartRate);
    ui->customPlot->replot();

    // calculate new coherence score
    float newCoherenceScore = currentSession->updateSession(newHeartRate, currentHRvector);

    // update achievement score text
    if (newCoherenceScore != -1) {
        float rounded = round(currentSession->getAchievementScore() * 10.0f) / 10.0f;
        ui->achvScoreBar->setText(QString::number(rounded));
        rounded = round(newCoherenceScore * 10.0f) / 10.0f;
        ui->coherenceBar->setText(QString::number(rounded));

        // determine light to turn on
        switch(currentSession->determineScoreLevel(newCoherenceScore)) {
            case 0:
                toggleRedLED();
                break;
            case 1:
                toggleBlueLED();
                break;
            case 2:
                toggleGreenLED();
                break;
        }
    }
    updatePacer();

}

/**
 * @brief Decreases the battery level by a small fixed amount and updates the UI accordingly.
 *
 * 1. Calculates the new battery level by subtracting a fixed amount (0.05) from the current level.
 * 2. Calls the changeBatteryLevel() function to update the battery level and UI elements.
 */
void MainWindow::drainBattery() {
    double newBatteryLevel = profile->getBLvl()-0.05;
    changeBatteryLevel(newBatteryLevel);
}

/**
 * @brief Adjusts the behavior of the timer based on whether the device is on the skin or not.
 * @param checked A boolean value indicating if the sensor is applied to the skin (true) or not (false).
 *
 * This function is responsible for the following actions:
 * 1. If the timer is not running and the device is applied to the skin and the ok button is pressed, it starts the timer.
 * 2. If the timer is running and the device is removed from the skin or the ok button is pressed, it stops the timer and displays the session summary.
 */
void MainWindow::applyToSkin(bool checked) {
    // if the timer is not running
    if (currentTimerCount != -1) {

        // is it on skin
        if (startSession) {
            if (!checked && (currentTimerCount > 0)) {
                displaySummary(currentSession, false);
            }
            else if (checked) {
                currentSession->getTimer()->start(1000);
            }
        } else if (!startSession && checked && (currentTimerCount > 0)) {
            displaySummary(currentSession, false);
        }
    }
}

/**
 * @brief Displays the summary of a session after it has been completed or when viewing session history.
 * @param session A pointer to the Session object containing the session data.
 * @param is_history A boolean value indicating if the session we are presenting a log (true) or a session summary after completion (false).
 *
 * This function is responsible for the following actions:
 * 1. Stops the timer and disconnects it if the session is not a log.
 * 2. Hides the session graph and calculates and sets the range for the summary graph.
 * 3. Populates the summary graph with session data.
 * 4. Creates a Log object, adds it to the database if not already a log, and updates the summary labels with relevant data.
 * 5. Calculates and displays coherence percentages for High, Medium, and Low coherence states.
 * 6. If not a log, resets the timer and various variables, and turns off the lights.
 */
void MainWindow::displaySummary(Session* session, bool is_history) {
    // stop timer
    if (!is_history) {
        currentSession->getTimer()->stop();
        currentSession->getTimer()->disconnect();
    }

    sessionSummaryVisible = true;

    // display summary graph
    ui->sessionFrame->setVisible(false);
    ui->customPlot_2->xAxis->setRange(0, session->getElapsedTime());


    // calculate min and max
    int sessionMin;
    int sessionMax;
    for (int i = 0; i < session->getGraph_int().size(); i++) {
        if (i == 0) {
            sessionMin = session->getGraph_int()[i];
            sessionMax = session->getGraph_int()[i];
        } else {
            if (sessionMin > session->getGraph_int()[i]) {
                sessionMin = session->getGraph_int()[i];
            }
            if (sessionMax < session->getGraph_int()[i]) {
                sessionMax = session->getGraph_int()[i];
            }
        }
    }

    ui->customPlot_2->yAxis->setRange(sessionMin, sessionMax);
    QVector<double> emptyData;
    ui->customPlot_2->graph(0)->setData(emptyData, emptyData);
    QVector<double> seconds;
    for (int i = 0; i <= session->getElapsedTime(); ++i) {
        seconds.append(static_cast<double>(i));
    }
    ui->customPlot_2->graph(0)->setData(seconds, session->getGraph_double());
    ui->customPlot_2->replot();

    Log *log = new Log(session, 0);
    if (!is_history){
        dbmanager->addLog(log);
    }

    // update labels
    float avgScore = (session->getAchievementScore()/session->getCoherenceCount());
    float rounded = round(avgScore * 10.0f) / 10.0f;
    if (session->getCoherenceCount() >0) {
        ui->avgScore->setText("Avg Score: "+ QString::number(rounded));
        ui->challengeLvlBar->setText(QString::number(session->getChallengeLevel()));
        rounded = round(session->getAchievementScore() * 10.0f) / 10.0f;
        ui->achvScoreBar_2->setText(QString::number(rounded));
        ui->lengthBar_2->setText(QString::number(session->getElapsedTime()) + "s");
    }

    // calculate precentages
    if (session->getCoherenceCount() > 0) {
        ui->timeInHigh->setText("% High: " + QString::number(round(log->getHighCoherencePercentage() * 10.0f) / 10.0f));
        ui->timeInMed->setText("% Med: " + QString::number(round(log->getMedCoherencePercentage() * 10.0f) / 10.0f));
        ui->timeInLow->setText("% Low: " + QString::number(round(log->getLowCoherencePercentage() * 10.0f) / 10.0f));
    } else {
        ui->timeInHigh->setText("% High: N/A");
        ui->timeInMed->setText("% Med: N/A");
        ui->timeInLow->setText("% Low: N/A");
    }

    delete log;

    if (session->getElapsedTime() > 0) {
        ui->summaryFrame->setVisible(true);
    } else {
        clearSessionSummary();
    }

    // reset timer
    if (!is_history) {
        this->currentTimerCount = -1;
        pacerCounter = -1;
        pacerCountDown = false;
        pacerWait = false;
        pacerCountUp = true;
        startSession = false;
        vectorHRcount = 10;
    }

    turnOffLights();
}

/**
 * @brief Clears the session summary and resets the session UI elements.
 *
 * This function is responsible for the following actions:
 * 1. Hides the summary frame and sets sessionSummaryVisible to false.
 * 2. Resets the Coherence, Length, and Achievement Score UI elements to their initial values.
 * 3. Resets the maxHR and minHR variables, and sets the ranges for the x and y axes of the session graph.
 * 4. Clears the data in the session graph and re-plots it.
 * 5. If inSessionView is true, navigates back to the main menu.
 */
void MainWindow::clearSessionSummary() {

    ui->summaryFrame->setVisible(false);
    sessionSummaryVisible = false;
    ui->coherenceBar->setText("0.0");
    ui->lengthBar->setText("0s");
    ui->achvScoreBar->setText("0.0");
    maxHR = 103;
    minHR = 57;
    ui->customPlot->xAxis->setRange(0, 5);
    ui->customPlot->yAxis->setRange(minHR, maxHR);
    QVector<double> emptyData;
    ui->customPlot->graph(0)->setData(emptyData, emptyData);
    ui->customPlot->replot();
    if (inSessionView) {
        inSessionView = false;
        navigateToMainMenu();
    }
}

/**
 * @brief Toggles the Red LED on and turns off the Green and Blue LEDs.
 */
void MainWindow::toggleRedLED() {
    ui->redLED->setStyleSheet(redOn);

    ui->greenLED->setStyleSheet(greenOff);
    ui->blueLED->setStyleSheet(blueOff);

}

/**
 * @brief Toggles the Blue LED on and turns off the Green and Red LEDs.
 */
void MainWindow::toggleBlueLED() {
    ui->blueLED->setStyleSheet(blueOn);

    ui->greenLED->setStyleSheet(greenOff);
    ui->redLED->setStyleSheet(redOff);
}

/**
 * @brief Toggles the Green LED on and turns off the Red and Blue LEDs.
 */
void MainWindow::toggleGreenLED() {
    ui->greenLED->setStyleSheet(greenOn);

    ui->redLED->setStyleSheet(redOff);
    ui->blueLED->setStyleSheet(blueOff);
}

/**
 * @brief Updates the pacer bar's value based on the current pacer state.
 *
 * This function manages three pacer stages: Count Up, Wait, and Count Down.
 * In the Count Up stage, the pacer bar fills up from 0 to 100 over 6 iterations.
 * In the Wait stage, the pacer holds its filled state for the pacer duration set by the device or the user.
 * In the Count Down stage, the pacer bar empties from 100 to 0 over 5 iterations.
 * The function cycles through these stages to create a continuous breathing pacer.
 */
void MainWindow::updatePacer() {
    if (pacerCountUp) {
        if (pacerCounter < 6) {
            ui->pacerBar->setValue(pacerCounter*20);
            pacerCounter++;
        } else {
            pacerCountUp = false;
            pacerWait = true;
            pacerCounter = 0;
        }
    } else if (pacerWait) {
        if (pacerCounter != pacer_dur) {
            pacerCounter++;
        } else {
            pacerWait = false;
            pacerCountDown = true;
            pacerCounter = 5;
        }
    } else if (pacerCountDown) {
        if (pacerCounter != 0) {
            pacerCounter--;
            ui->pacerBar->setValue(pacerCounter*20);
        } else {
            pacerCountDown = false;
            pacerCountUp = true;
            pacerCounter = 0;
        }
    }
}

/**
 * @brief Generates a heart rate value using a pre-defined heart rate data array.
 *
 * This function selects a random heart rate vector from the pre-defined heart rate data array
 * and returns the heart rate value at the current index within that vector.
 * It cycles through the vector and resets the index when it reaches the end.
 * The random vector selection occurs every time the index resets.
 *
 * @return The heart rate value at the current index of the selected vector.
 */
int MainWindow::generateHR() {
    if (vectorHRcount > 9) {
        vectorHRcount = 0;
        int min = 3;    // first 3 are debug
        int max = 28;
        int randomNumberInRange = min + (std::rand() % (max - min + 1));
        currentHRvector = randomNumberInRange;
    }
    return heartRateData[currentHRvector][vectorHRcount++];

}

/**
 * @brief Turns off all LED lights and the heart rate connection indicator if the device is powered off.
 */
void MainWindow::turnOffLights() {
    ui->redLED->setStyleSheet(redOff);
    ui->blueLED->setStyleSheet(blueOff);
    ui->greenLED->setStyleSheet(greenOff);
    if (!powerStatus) {ui->hrConnection->setStyleSheet(connectionOff);}
}

/**
 * @brief Toggles the skin connection status and updates the UI accordingly.
 */
void MainWindow::toggleSkin() {
    connectedStatus = !connectedStatus;
    if (connectedStatus && powerStatus) {
        ui->hrConnection->setStyleSheet(connectionOn);
    } else {
        ui->hrConnection->setStyleSheet(connectionOff);
    }
    applyToSkin(connectedStatus);
}

/**
 * @brief Drops all tables in the database using the database manager.
 */
void MainWindow::dropTables() {
    dbmanager->dropTables();
}
