#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    // init settings
    pacer_dur = 10;
//    challenge_level = 1;
    challenge_level = 4; // delete this before we hand it in lol
    pacerCounter = -1;

    // Set initial Skin status
    connectedStatus = false;

    // Initialize the timer
    currentTimerCount = -1;

    // create database dbmanager
    dbmanager = new DBManager();

    // create profile
    profile = dbmanager->getProfile(1);

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
    connect(ui->chargeBatteryButton, &QPushButton::released, this, &MainWindow::rechargeBattery);

    // TODO: connect SpinBox to set the battery level
    connect(ui->batteryLevelAdminSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::changeBatteryLevel);

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
    ui->batteryLevelAdminSpinBox->setValue(profile->getBLvl());

    connect(ui->redButton, SIGNAL(released()), this, SLOT (toggleRedLED()));
    connect(ui->greenButton, SIGNAL(released()), this, SLOT (toggleGreenLED()));
    connect(ui->blueButton, SIGNAL(released()), this, SLOT (toggleBlueLED()));
    redOn = "background-color: rgb(220, 0, 0)";
    redOff = "background-color: rgb(80, 0, 0)";
    greenOn = "background-color: rgb(0, 170, 0)";
    greenOff = "background-color: rgb(0, 50, 0)";
    blueOn = "background-color: rgb(0, 0, 230)";
    blueOff = "background-color: rgb(0, 0, 80)";
    turnOffLights();

    // set session ui to invisible
    // TODO: add session ui items
    //ui->programViewWidget->setVisible(false);
    //ui->electrodeLabel->setVisible(false);
    std::srand(static_cast<unsigned>(std::time(0)));

    // temp
    connectedStatus = true;

    // setup graphs
    ui->sessionFrame->setVisible(false);
    ui->summaryFrame->setVisible(false);
    maxHR = 110;
    minHR = 50;
    ui->customPlot->xAxis->setRange(0, 5);
    ui->customPlot->yAxis->setRange(minHR, maxHR);
    ui->customPlot->xAxis->setLabel("Time (s)");
    ui->customPlot->yAxis->setLabel("HR");
    ui->customPlot->addGraph(); // active session graph
    ui->customPlot->graph(0)->setPen(QPen(Qt::red)); // set the pen color
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine); // set the line style
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssNone);

    ui->customPlot_2->xAxis->setLabel("Time (s)");
    ui->customPlot_2->yAxis->setLabel("HR");
    ui->customPlot_2->addGraph(); // active session graph
    ui->customPlot_2->graph(0)->setPen(QPen(Qt::red)); // set the pen color
    ui->customPlot_2->graph(0)->setLineStyle(QCPGraph::lsLine); // set the line style
    ui->customPlot_2->graph(0)->setScatterStyle(QCPScatterStyle::ssNone);

    sessionSummaryVisible = false;
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
    } else if (sessionSummaryVisible) {
        clearSessionSummary();
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

    } else if (sessionSummaryVisible) {
        clearSessionSummary();
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
        displaySummary();
        clearSessionSummary();
    }
}

// Toggle visibilty of the menu
void MainWindow::changePowerStatus() {
    if (!powerStatus) {turnOffLights();}
    //activeQListWidget->setVisible(powerStatus);
    //ui->menuLabel->setVisible(powerStatus);

    ui->screen->setVisible(powerStatus); // Sets the whole screen widget's and all children's visibility

    //Remove this if we want the menu to stay in the same position when the power is off
    if (powerStatus) {
        MainWindow::navigateToMainMenu();
        //applyToSkin(false);
    }

    ui->upButton->setEnabled(powerStatus);
    ui->downButton->setEnabled(powerStatus);
    ui->leftButton->setEnabled(powerStatus);
    ui->rightButton->setEnabled(powerStatus);
    ui->menuButton->setEnabled(powerStatus);
    ui->okButton->setEnabled(powerStatus);
    ui->backButton->setEnabled(powerStatus);
}

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
    timeString = QString::number(currentTimerCount) + "s";
    // TODO: update screen
    //scene->addText(timeString);
    init_timer(timer);

    // create session
    int thisSessionID = profile->increaseSessAmt();
    currentSession = new Session(thisSessionID, challenge_level, pacer_dur, QDateTime::currentDateTime(), timer);
    qDebug() << currentSession->getSessionNum();
}

void MainWindow::init_timer(QTimer* timer){
    connect(timer, &QTimer::timeout, this, &MainWindow::update_timer);

    if (connectedStatus){
        timer->start(1000);
    }
}

void MainWindow::update_timer(){
    drainBattery();

    // update duration text
    ui->lengthBar->setText(QString::number(currentTimerCount) + "s");
    //ui->treatmentView->scene()->clear();
    //ui->treatmentView->scene()->addText(timeString);

    currentTimerCount++;

    // TODO: get new heart rate from table?
    int newHeartRate = generateHR();   // some function should be here to set this value. the function could look up an array heart rates based of currentTimerCount

    // update y axis
    if (newHeartRate < minHR) {
        minHR = newHeartRate-10;
    } else if (newHeartRate > maxHR) {
        maxHR = newHeartRate+10;
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
    float newCoherenceScore = currentSession->updateSession(newHeartRate);

    // update achievement score text
    if (newCoherenceScore != -1) {
        float rounded = round(currentSession->getAchievementScore() * 10.0f) / 10.0f;
        ui->achvScoreBar->setText(QString::number(rounded));

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
            qDebug() << "Starting timer";
            currentSession->getTimer()->start(1000);
        }
    }
}

void MainWindow::displaySummary() {
    // stop timer
    currentSession->getTimer()->stop();
    currentSession->getTimer()->disconnect();

    sessionSummaryVisible = true;

    // display summary graph
    ui->sessionFrame->setVisible(false);
    ui->customPlot_2->xAxis->setRange(0, currentTimerCount);
    ui->customPlot_2->yAxis->setRange(minHR, maxHR);
    QVector<double> emptyData;
    ui->customPlot_2->graph(0)->setData(emptyData, emptyData);
    QVector<double> seconds;
    for (int i = 0; i <= currentTimerCount; ++i) {
        seconds.append(static_cast<double>(i));
    }
    ui->customPlot_2->graph(0)->setData(seconds, currentSession->getGraph());
    ui->customPlot_2->replot();

    // TODO: save session into dbmanager
    Log *log = new Log(this->currentSession, profile->getId());
    dbmanager->addLog(log);

    // update labels
    float avgScore = (currentSession->getAchievementScore()/currentSession->getCoherenceCount());
    float rounded = round(avgScore * 10.0f) / 10.0f;
    ui->avgScore->setText("Avg Score: "+ QString::number(rounded));
    ui->challengeLvlBar->setText(QString::number(currentSession->getChallengeLevel()));
    rounded = round(currentSession->getAchievementScore() * 10.0f) / 10.0f;
    ui->achvScoreBar_2->setText(QString::number(rounded));
    ui->lengthBar_2->setText(QString::number(currentSession->getElapsedTime()) + "s");
    ui->timeInHigh->setText("% High: " + QString::number(log->getHighCoherencePercentage()));
    ui->timeInMed->setText("% Med: " + QString::number(log->getMedCoherencePercentage()));
    ui->timeInLow->setText("% Low: " + QString::number(log->getLowCoherencePercentage()));

    delete log;

    ui->summaryFrame->setVisible(true);

    // reset timer
    this->currentTimerCount = -1;
    pacerCounter = -1;
    pacerCountDown = false;
    pacerWait = false;
    pacerCountUp = true;



    turnOffLights();
    // TODO: session data varaibles to 0
}

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
}

void MainWindow::toggleRedLED() {
    // TODO: Change colour of red led to on
    ui->redLED->setStyleSheet(redOn);

    // TODO: change colour of green and blue led to off
    ui->greenLED->setStyleSheet(greenOff);
    ui->blueLED->setStyleSheet(blueOff);

}

void MainWindow::toggleBlueLED() {
    // TODO: Change colour of blue led to on
    ui->blueLED->setStyleSheet(blueOn);

    // TODO: change colour of red and green led to off
    ui->greenLED->setStyleSheet(greenOff);
    ui->redLED->setStyleSheet(redOff);
}

void MainWindow::toggleGreenLED() {
    // TODO: Change colour of green led to on
    ui->greenLED->setStyleSheet(greenOn);

    // TODO: change colour of red and blue led to off
    ui->redLED->setStyleSheet(redOff);
    ui->blueLED->setStyleSheet(blueOff);
}

void MainWindow::updatePacer() {
    // TODO: timing seems a little off might wanna look into it
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


int MainWindow::generateHR() {
    int min = 50;
    int max = 120;
    int randomNumberInRange = min + (std::rand() % (max - min + 1));
    return randomNumberInRange;
}

void MainWindow::turnOffLights() {
    ui->redLED->setStyleSheet(redOff);
    ui->blueLED->setStyleSheet(blueOff);
    ui->greenLED->setStyleSheet(greenOff);
}
