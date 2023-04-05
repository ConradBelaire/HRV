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

    // create database dbmanager
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
    std::srand(static_cast<unsigned>(std::time(0)));
}


MainWindow::~MainWindow() {

    dbmanager->addProfile(profile->getId(), profile->getBLvl(), profile->getSessAmt());

    delete mainMenuOG;
    delete ui;

    for (int i = 0; i < sessions.size(); i++) {
        delete sessions[i];
    }

    delete dbmanager;
    delete profile;
}

QString MainWindow::floatToStringWithOneDecimalPlace(float value) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << value;
    return QString::fromStdString(ss.str());
}

Menu* MainWindow::create_history_menu(Menu* m) {
        // initialise session list
        QStringList sessionList;
        sessionList.append("CLEAR");

        QVector<Log*>* logs = dbmanager->getLogs(profile->getId());
        for (Log* currentLog : *logs) {
            sessionList.append(QString::number(currentLog->getSessionTime()));
        }
        //create parent menu
        Menu* history = new Menu("HISTORY", sessionList, m);

        // create child menus
        Menu* clearHistory = new Menu("CLEAR", {"YES","NO"}, history);
        history->addChildMenu(clearHistory);

        for (Log* currentLog : *logs) {
            Menu* session_menu = new Menu(QString::number(currentLog->getId()), {"VIEW", "DELETE"}, history);
            Menu* view = new Menu("VIEW", {
                "Session Number: " + QString::number(currentLog->getId()),
                "Date: " + currentLog->getDate(),
                "Session time: " + QString::number(currentLog->getSessionTime()),
                "Challenge Level: " + QString::number(currentLog->getChallengeLevel()),
                "Pacer Duration: " + QString::number(currentLog->getPacerDuration()),
                "Coherence Average: " + floatToStringWithOneDecimalPlace(currentLog->getAvgCoherence()),
                "Achievement Score: " + floatToStringWithOneDecimalPlace(currentLog->getAchievementScore()),
                "Low Coherence Percentage: " + floatToStringWithOneDecimalPlace(currentLog->getLowCoherencePercentage()),
                "Medium Coherence Percentage: " + floatToStringWithOneDecimalPlace(currentLog->getMedCoherencePercentage()),
                "High Coherence Percentage: " + floatToStringWithOneDecimalPlace(currentLog->getHighCoherencePercentage()),

            }, session_menu);
            Menu* delete_menu = new Menu("DELETE", {"YES","NO"}, session_menu);
            session_menu->addChildMenu(view);
            session_menu->addChildMenu(delete_menu);

            history->addChildMenu(session_menu);
        }
        return history;
}

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

void MainWindow::initializeMainMenu(Menu* m) {
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

// fucntion to determine if session number is real
bool MainWindow::is_session_num(QString log_id){
    return dbmanager->doesLogExist(log_id.toInt());
}

// pressing the ok button
void MainWindow::navigateSubMenu() {

    int index = activeQListWidget->currentRow();
    if (index < 0) return;

    // navigate to begin_session menu
    if(masterMenu->getChildMenu(index)->getName() == "BEGIN SESSION") {
        masterMenu = masterMenu->getChildMenu(index);
        MainWindow::updateMenu(masterMenu->getName(), {});
        MainWindow::start_session();
        return;
    }

    // navigate to history menu
    if (masterMenu->getChildMenu(index)->getName() == "HISTORY") {
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }

    // delete session
    if(is_session_num(masterMenu->getChildMenu(index)->getName())){
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }

    // navigate into the view menu
    if (masterMenu->getChildMenu(index)->getName() == "VIEW") {
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }

    // navigate into the delete menu
    if (masterMenu->getChildMenu(index)->getName() == "DELETE") {
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }

    // fucntionality of the delete menu
    if(masterMenu->getParent()->getName() == "DELETE"){
        if (masterMenu->getMenuItems()[index] == "YES") {
            int log_id = masterMenu->getParent()->getParent()->getName().toInt();
            dbmanager->deleteLog(log_id);
            navigateBack();
            return;
        }
        else {
            navigateBack();
            return;
        }
    }

    // navigate to clear menu
    if (masterMenu->getChildMenu(index)->getName() == "CLEAR") {
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }

    // fucntionality of the clear menu
    if (masterMenu->getParent()->getName() == "CLEAR"){
        if (masterMenu->getMenuItems()[index] == "YES") {
            dbmanager->deleteLogs();
            navigateBack();
            return;
        }
        else {
            navigateBack();
            return;
        }
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

    // fucntionality of the reset menu
    if(masterMenu->getParent()->getName() == "RESET"){
        if (masterMenu->getMenuItems()[index] == "YES") {
            dbmanager->deleteLogs();
            challenge_level = 1;
            pacer_dur = 10;
            navigateBack();
            return;
        }
        else {
            navigateBack();
            return;
        }
    }

    // navigate to challenge menu
    if (masterMenu->getChildMenu(index)->getName() == "CHALLENGE LEVEL") {
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }

    // navigate to challenge menu
    if(masterMenu->getParent()->getName() == "CHALLENGE LEVEL"){
        challenge_level = index + 1;
    }

    // navigate to Pacer duration menu
    if (masterMenu->getChildMenu(index)->getName() == "PACER DURATION") {
        masterMenu = masterMenu->getChildMenu(index);
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
        return;
    }

    // navigate to Pacer duration menu
    if(masterMenu->getParent()->getName() == "PACER DURATION"){
        pacer_dur = index + 1;
    }
}

void MainWindow::updateMenu(const QString &selectedMenuItem, const QStringList &menuItems) {

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
        applyToSkin(false);
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
    // TODO: update screen
    //scene->addText(timeString);
    init_timer(timer);

    // create session
    currentSession = new Session(profile->getSessAmt(), challenge_level, pacer_dur, QDateTime::currentDateTime(), timer);
}

void MainWindow::init_timer(QTimer* timer){
    connect(timer, &QTimer::timeout, this, &MainWindow::update_timer);

    if (connectedStatus){
        timer->start(1000);
    }
}

void MainWindow::update_timer(){
    drainBattery();
    timeString = QString::number(currentTimerCount) + "s";
    //ui->treatmentView->scene()->clear();
    //ui->treatmentView->scene()->addText(timeString);

    currentTimerCount++;

    // TODO: get new heart rate from table?
    int newHeartRate = generateHR();   // some function should be here to set this value. the function could look up an array heart rates based of currentTimerCount
    // calculate new coherence score
    float newCoherenceScore = currentSession->updateSession(newHeartRate);

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

    // TODO: update pacer
    updatePacer();

}

void MainWindow::drainBattery() {
    double newBatteryLevel = profile->getBLvl()-0.05;
    changeBatteryLevel(newBatteryLevel);
}



// TODO?:
void MainWindow::applyToSkin(bool checked) {
    // TODO: update screen
    // ui->electrodeLabel->setPixmap(QPixmap(checked ? ":/icons/electrodeOn.svg" : ":/icons/electrodeOff.svg"));
    // ui->applyToSkinAdminBox->setCurrentIndex(checked ? 1 : 0);
    bool onSkin = checked; // why?

    // if the timer is not running
    if (currentTimerCount != -1) {

        // is it on skin
        if (!onSkin) {
            displaySummary();
        }
        else {
            currentSession->getTimer()->start(1000);
        }
    }
}

void MainWindow::displaySummary() {
    currentSession->getTimer()->stop();
    // TODO: save session into dbmanager
    Log *log = new Log(this->currentSession, profile->getId());
    dbmanager->addLog(log);

    delete log;
    // reset timer
    this->currentTimerCount = -1;

    currentSession->getTimer()->stop();
    currentSession->getTimer()->disconnect();
    // TODO: session data varaibles to 0
    // need to take in data for this

    // TODO: make session ui to invisible
    // TODO: make session summary visible
}

void MainWindow::toggleRedLED() {
    // TODO: Change colour of red led to on

    // TODO: change colour of green and blue led to off
}

void MainWindow::toggleBlueLED() {
    // TODO: Change colour of blue led to on

    // TODO: change colour of red and green led to off
}

void MainWindow::toggleGreenLED() {
    // TODO: Change colour of green led to on

    // TODO: change colour of red and blue led to off
}

void MainWindow::updatePacer() {
    // TODO: connect pacer once implemented

    // math for paccer
    int pacerBarValue = currentTimerCount / currentSession->getPacerDuration();
    // TODO: set progress bar here

}


int MainWindow::generateHR() {
    int min = 60;
    int max = 100;
    int randomNumberInRange = min + (std::rand() % (max - min + 1));
    return randomNumberInRange;
}
