#include "session.h"

Session::Session(
    int sessionNum,
    int challengeLevel,
    int pacerDuration,
    QTimer* timer) :
    SESSION_NUM(sessionNum),
    CHALLENGE_LEVEL(challengeLevel),
    PACER_DURATION(pacerDuration),
    currentHR(0),
    timeInLow(0),
    timeInMed(0),
    timeInHigh(0),
    elapsedTime(0),
    countUpdates(0),
    coherenceCount(0),
    coherenceSum(0),
    currentCoherence(0),
    timer(timer)    {
    coherenceScoreData = {0.4, 2.5, 4.5, 0.7, 1.3, 1.8, 3.5, 2.9, 4.2, 1.5, 6.0, 1.2, 3.8, 2.2, 1.9, 5.5, 2.7, 4.0, 0.9, 6.2, 1.4, 3.3, 2.8, 0.6, 5.2, 1.6, 3.5, 2.4, 0.8};
}

Session::Session(
    Log* log) :
    SESSION_NUM(log->getId()),
    CHALLENGE_LEVEL(log->getChallengeLevel()),
    PACER_DURATION(0),
    timeInLow(log->getIsLow()),
    timeInMed(log->getIsMed()),
    timeInHigh(log->getIsHigh()),
    elapsedTime(log->getSessionTime()),
    coherenceCount(log->getCoherenceCount()),
    coherenceSum(log->getAchievementScore()),
    recordedHR(log->getHeartRates_double()) {
}

// getters
int Session::getTimeLow() const {return timeInLow;}
int Session::getTimeMed() const {return timeInMed;}
int Session::getTimeHigh() const {return timeInHigh;}
int Session::getSessionNum() const {return SESSION_NUM;}
int Session::getElapsedTime() const {return elapsedTime;}
int Session::getPacerDuration() const {return PACER_DURATION;}
int Session::getChallengeLevel() const {return CHALLENGE_LEVEL;}
int Session::getCoherenceCount() const {return coherenceCount;}
int Session::getCurrentHeartRate() const {return currentHR;}

float Session::getCurrentCoherence() const {return currentCoherence;}
float Session::getAchievementScore() const {return coherenceSum;}

QTimer* Session::getTimer() {return timer;}
QVector<double> Session::getGraph_double() const {return recordedHR;}
QVector<int> Session::getGraph_int() const {
    QVector<int> list_of_ints;
    for (double value : recordedHR) {
        list_of_ints.append(static_cast<int>(value));
    }
    return list_of_ints;
}

// setters
void Session::addToLow() {
    timeInLow++;
    if (timeInLow == 1) {
        qDebug() << "*BEEP* (low coherence light turned on for the first time)";
    }
}
void Session::addToMed() {
    timeInMed++;
    if (timeInMed == 1) {
        qDebug() << "*BEEP* (medium coherence light turned on for the first time)";
    }
}
void Session::addToHigh() {
    timeInHigh++;
    if (timeInHigh == 1) {
        qDebug() << "*BEEP* (high coherence light turned on for the first time)";
    }
}

void Session::setAchievementScore(float newAchievementScore) {coherenceSum = newAchievementScore;}
void Session::setLowCoherencePercentage(float newLowCoherencePercentage) {timeInLow = newLowCoherencePercentage;}
void Session::setMedCoherencePercentage(float newMedCoherencePercentage) {timeInMed = newMedCoherencePercentage;}
void Session::setHighCoherencePercentage(float newHighCoherencePercentage) {timeInHigh = newHighCoherencePercentage;}
void Session::setHeartRates_double(QVector<double> newHeartRates_double) {recordedHR = newHeartRates_double;}

// functions
float Session::updateSession(int newHR, int currentVector) {
    currentHR = newHR;
    elapsedTime++;
    recordedHR.append(newHR);

    if (countUpdates == 5) {
        countUpdates = 0;
        return calculateCoherenceScore(currentVector);;
    } else {
        countUpdates++;
        return -1;
    }
}

int Session::determineScoreLevel(float cohernceScore) {
    switch(CHALLENGE_LEVEL) {
        case 1:
            if (0 <= cohernceScore && cohernceScore <= 0.5) {return 0;}
            else if (0.6 <= cohernceScore && cohernceScore <= 0.9) {return 1;}
            else if (1.0 <= cohernceScore && cohernceScore <= 16) {return 2;}
            else {return -1;}
        case 2:
            if (0 <= cohernceScore && cohernceScore <= 0.6) {return 0;}
            else if (0.7 <= cohernceScore && cohernceScore <= 2.1) {return 1;}
            else if (2.2 <= cohernceScore && cohernceScore <= 16) {return 2;}
            else {return -1;}
        case 3:
            if (0 <= cohernceScore && cohernceScore <= 1.8) {return 0;}
            else if (1.9 <= cohernceScore && cohernceScore <= 4.0) {return 1;}
            else if (4.1 <= cohernceScore && cohernceScore <= 16) {return 2;}
            else {return -1;}
        case 4:
            if (0 <= cohernceScore && cohernceScore <= 4.0) {return 0;}
            else if (4.1 <= cohernceScore && cohernceScore <= 6.0) {return 1;}
            else if (6.1 <= cohernceScore && cohernceScore <= 16) {return 2;}
            else {return -1;}
        default:
            return -1;
    }
}

// private functions
void Session::addCoherenceScore(float newCoherenceScore) {
    coherenceSum += newCoherenceScore;
    coherenceCount++;
}

float Session::calculateCoherenceScore(int currentVector) {
    currentCoherence = coherenceScoreData[currentVector];
    qDebug() << "New Coherence Score: " + QString::number(currentCoherence);

    int rank = determineScoreLevel(currentCoherence);
    switch(rank) {
        case 0:
            addToLow();
            break;
        case 1:
            addToMed();
            break;
        case 2:
            addToHigh();
            break;
    }
    addCoherenceScore(currentCoherence);
    return currentCoherence;
}
