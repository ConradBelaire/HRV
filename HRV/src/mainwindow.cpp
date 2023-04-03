#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    // init settings
    pacer_dur = 10;
    challenge_level = 1;

    // Set initial Skin status
    connectedStatus = false;

    // Initialize the timer
    currentTimerCount = -1;

    // create database manager
    dbmanager = new DBManager();
    
    // create profile
    profile = dbmanager->getProfile(1);

    //maxPower = 100;

    // Initialize the menu
    masterMenu = new Menu("MAIN MENU", {"BEGIN SESSION","HISTORY","SETTINGS"}, nullptr);
    mainMenuOG = masterMenu;
    initializeMainMenu(masterMenu);

    // Initialize the main menu view
    activeQListWidget = ui->mainMenuListView;
    activeQListWidget->addItems(masterMenu->getMenuItems());
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(masterMenu->getName());

    powerStatus = false;
    changePowerStatus();
    // TODO: connect power button
    connect(ui->powerButton, &QPushButton::released, this, &MainWindow::powerChange);

    // TODO: connect charge button
    //connect(ui->chargeAdminButton, &QPushButton::released, this, &MainWindow::rechargeBattery);

    // TODO: connect SpinBox to set the battery level
    //connect(ui->batteryLevelAdminSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::changeBatteryLevel);

    // TODO: connect the menu buttons
    // TODO?: maybe apply a skin to these buttons
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
    connect(ui->okButton, &QPushButton::pressed, this, &MainWindow::navigateSubMenu);
    connect(ui->menuButton, &QPushButton::pressed, this, &MainWindow::navigateToMainMenu);
    connect(ui->backButton, &QPushButton::pressed, this, &MainWindow::navigateBack);


    // TODO?: apply more skins

    // TODO: connect the power level spin box
    //connect(ui->powerLevelAdminSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::changePowerLevel);

    // Initialize battery levels
    //ui->powerLevelAdminSpinBox->setValue(profile->getPLvl());
    //ui->batteryLevelAdminSpinBox->setValue(profile->getBLvl());

    // set session ui to invisible
    // TODO: add session ui items
    //ui->programViewWidget->setVisible(false);
    //ui->electrodeLabel->setVisible(false);
}


MainWindow::~MainWindow() {

    db->addProfile(profile->getId(), profile->getBLvl(), profile->getSessAmt());

    delete mainMenuOG;
    delete ui;

    for (int i = 0; i < sessions.size(); i++) {
        delete sessions[i];
    }

    delete db;
    delete profile;
}


void MainWindow::initializeMainMenu(Menu* m) {

    QStringList sessionList;
    sessionList.append("CLEAR");

    for (Session* s : this->sessions) {
        sessionList.append(s->getName());
    }

void MainWindow::initializeMainMenu(Menu* m) {
    // create begin_session menu
    m->addChildMenu(begin_session);
    Menu* begin_session = new Menu("BEGIN SESSION", {}, m);

    //create history menu
    Menu* history = create_history_menu();
    m->addChildMenu(history);

    Menu* settings = new Menu("SETTINGS", {"RESET", "CHALLENGE LEVEL", "PACER DURATION"}, m);
    m->addChildMenu(settings);

    Menu* settings = create_settings_menu();

    Menu* create_settings_menu(){
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
    }

    Menu* create_history_menu() {
        // initialise session list
        QStringList sessionList;
        sessionList.append("CLEAR");

        for (log* s : dbmanager->getProfileLogs(profile->getId())) {
            sessionList.append(s->getName());
        }
        //create parent menu
        Menu* history = new Menu("HISTORY", sessionList, m);

        // create child menus
        Menu* clearHistory = new Menu("CLEAR", {"YES","NO"}, history);
        history->addChildMenu(clearHistory);

        for (Session* s : this->sessions) {
            Menu* session_menu = new Menu(s->getSessionNum(), {"VIEW", "DELETE"}, history);
            history->addChildMenu(session_menu);
        }
        return history;
    }
}

void MainWindow::navigateUpMenu() {

    int nextIndex = activeQListWidget->currentRow() - 1;

    if (nextIndex < 0) {
        nextIndex = activeQListWidget->count() - 1;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}

void MainWindow::navigateDownMenu() {

    int nextIndex = activeQListWidget->currentRow() + 1;

    if (nextIndex > activeQListWidget->count() - 1) {
        nextIndex = 0;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}


void MainWindow::navigateToMainMenu() {

    if (currentTimerCount != -1) {
        displaySummary();
    }

    // go to main menu
    while (masterMenu->getName() != "MAIN MENU") {
        masterMenu = masterMenu->getParent();
    }
    updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    //ui->programViewWidget->setVisible(false);
    //ui->electrodeLabel->setVisible(false);
}

// back button
void MainWindow::navigateBack() {
    if (currentTimerCount != -1) {
        displaySummary();

    }

    if (masterMenu->getName() == "MAIN MENU") {
        activeQListWidget->setCurrentRow(0);
    }
    else {
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }

    // set the session ui to invisible
    //ui->programViewWidget->setVisible(false);
    //ui->electrodeLabel->setVisible(false);
}


void MainWindow::navigateSubMenu() {

    int index = activeQListWidget->currentRow();
    if (index < 0) return;

    // Prevent crash if ok button is selected in view
    // TODO: Look into this when we have UI set up
    if (masterMenu->getName() == "VIEW") {
        return;
    }

    //Logic for when the menu is the delete menu.
    if (masterMenu->getName() == "CLEAR") {
        if (masterMenu->getMenuItems()[index] == "YES") {
            db->deleteSessions();

            navigateBack();
            return;
        }
        else {
            navigateBack();
            return;
        }
    }

    //Logic for when the menu is the delete specific session menu.

    if (masterMenu->getName() == "DELETE") {
        if (masterMenu->getMenuItems()[index] == "YES") {
            Session* session = db->getSesionByName(masterMenu->getParent()->getName());
            db->deleteSession(session);

            navigateBack();
            return;
        }
        else {
            navigateBack();
            return;
        }
    }

    if (masterMenu->getName() == "CHALLENGE LEVEL") {
        challenge_level = index + 1;

        // TODO: reset to 1 after 4
    }

    if (masterMenu->getName() == "PACER DURATION") {
        pacer_dur = index + 1;

        // TODO: reset to 1 after 30
    }

    if (masterMenu->getName() == "RESET") {
        if (masterMenu->getMenuItems()[index] == "YES") {
            challenge_level = 1;
            pacer_dur = 10;
            db->deleteProfiles();
            profile = db->getProfile(1);

            // TURN OFF
            MainWindow::powerChange();
            // TURN ON
            MainWindow::powerChange();    

            navigateBack();
            return;
        }
        else {
            navigateBack();
            return;
        }
    }

    //If the menu is a parent and clicking on it should display more menus.
    if (masterMenu->getChildMenu(index)->getMenuItems().length() > 0) {
        masterMenu = masterMenu->getChildMenu(index);
        MainWindow::updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }

    //If the menu is not a parent and clicking on it should start a Session
    else if (masterMenu->getChildMenu(index)->getMenuItems().length() == 0 && (masterMenu->getName() == "BEGIN SESSION")) {
        //Update new menu info
        masterMenu = masterMenu->getChildMenu(index);
        MainWindow::updateMenu(masterMenu->getName(), {});
        MainWindow::beginSession();

    }
}

void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) {

    activeQListWidget->clear();
    activeQListWidget->addItems(menuItems);
    activeQListWidget->setCurrentRow(0);

    ui->menuLabel->setText(selectedMenuItem);
}


// function will be used as a reference to the recharge button
void MainWindow::rechargeBattery(){
    changeBatteryLevel(100);
}

// function will set the battery level to the newLevel
void MainWindow::changeBatteryLevel(double newLevel) {

    if (newLevel >= 0.0 && newLevel <= 100.0) {
        if (newLevel == 0.0 && powerStatus == true) {
            powerChange();
            profile->setBLvl(0);
        }else{
            profile->setBLvl(newLevel);
        }

        //ui->batteryLevelAdminSpinBox->setValue(newLevel);
        int newLevelInt = int(newLevel);
        //ui->batteryLevelBar->setValue(newLevelInt);

        // does this even work?
        QString highBatteryHealth = "QProgressBar { selection-background-color: rgb(78, 154, 6); background-color: rgb(0, 0, 0); }";
        QString mediumBatteryHealth = "QProgressBar { selection-background-color: rgb(196, 160, 0); background-color: rgb(0, 0, 0); }";
        QString lowBatteryHealth = "QProgressBar { selection-background-color: rgb(164, 0, 0); background-color: rgb(0, 0, 0); }";

        if (newLevelInt >= 50) {
            //ui->batteryLevelBar->setStyleSheet(highBatteryHealth);
        }
        else if (newLevelInt >= 20) {
            //ui->batteryLevelBar->setStyleSheet(mediumBatteryHealth);
        }
        else {
            //ui->batteryLevelBar->setStyleSheet(lowBatteryHealth);
        }
    }
}


// change the power status variable
void MainWindow::powerChange(){

    // if the battery level is greater than 0, then toggle the power status
    if (profile->getBLvl() > 0) {
        powerStatus  = !powerStatus;
        changePowerStatus();
    }

    // if in the middle of a session
    if (currentTimerCount != -1){
        //Save Session
        applyToSkin(false)
    }
}

// Toggle visibilty of the menu
void MainWindow::changePowerStatus() {
    activeQListWidget->setVisible(powerStatus);
    ui->menuLabel->setVisible(powerStatus);
}

void MainWindow::start_session(){
    //make whatever sesions ui visible
    // ui->electrodeLabel->setVisible(true);


    profile->increaseSessAmt();

    currentTimerCount = 0;

    // initialize the timer
    timer = new QTimer(this);
    timeString = QString::number(currentTimerCount) + "s";
    scene->addText(timeString);
    initializeTimer(timer);

    // create session
    currentSession = new Session(profile->getSessAmt(), challenge_level, pacer_dur, QDateTime::currentDateTime());
}

void MainWindow::init_timer(QTimer* timer){
    connect(timer, &QTimer::timeout, this, &MainWindow::update_timer);

    if (connectedStatus){
        timer->start(1000);
    }
}

void MainWindow::update_timer(){
    DrainBattery();
    timeString = QString::number(currentTimerCount) + "s";
    //ui->treatmentView->scene()->clear();
    //ui->treatmentView->scene()->addText(timeString);

    currentTimerCount++;
    
    // TODO: update pacer


    // TODO: get heart rate


    // TODO: calculate/get coherence score


    // TODO: update the currentSession object
    //currentSession->

    // TODO: change coherence lights



}

void MainWindow::drainBattery() {
    double newBatteryLevel = profile->getBLvl()-0.05;
    changeBatteryLevel(newBatteryLevel);
}



// TODO: IMPLEMENT DB functinality
void MainWindow::applyToSkin(bool checked) {

    // ui->electrodeLabel->setPixmap(QPixmap(checked ? ":/icons/electrodeOn.svg" : ":/icons/electrodeOff.svg"));
    ui->applyToSkinAdminBox->setCurrentIndex(checked ? 1 : 0);
    bool onSkin = checked; // why?

    if (this->currentTimerCount != -1) {
        if (!onSkin) {
            displaySummary();
        }
        else {
            currentTherapy->getTimer()->start(1000);
        }
    }
}

void MainWindow::displaySummary() {
    currentTherapy->getTimer()->stop();
    // TODO: save session into db
    Log *log = new Log(this->currentSession)
                   db->addlog(
                       log->getId(),
                       log->getProfileId(),
                       log->getChallengeLevel(),
                       log->getIsLow(),
                       log->getIsMed(),
                       log->getIsHigh()
                           log->getAvgCoherence(),
                       log->getLogTime(),
                       log->getAchievementScore(),
                       log->getGraph(),
                       log->getDate());

    delete log;
    // reset timer
    this->currentTimerCount = -1;

    currentTherapy->getTimer()->stop();
    currentTherapy->getTimer()->disconnect();
    // TODO: session data varaibles to 0
    // need to take in data for this

    // TODO: make session ui to invisible
    // TODO: make session summary visible
}
