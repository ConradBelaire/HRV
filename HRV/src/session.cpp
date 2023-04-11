#include "session.h"

Session::Session(
    int sessionNum,
    int challengeLevel,
    int pacerDuration,
    const QDateTime& startTime,
    QTimer* timer) :
    SESSION_NUM(sessionNum),
    CHALLENGE_LEVEL(challengeLevel),
    PACER_DURATION(pacerDuration),
    START_TIME(startTime),
    currentHR(0),
    timeInLow(0),
    timeInMed(0),
    timeInHigh(0),
    elapsedTime(0),
    countUpdates(0),
    coherenceCount(0),
    coherenceSum(0),
    currentCoherence(0),
    timer(timer)   {
    std::srand(static_cast<unsigned>(std::time(0)));
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
QDateTime Session::getStartTime() const {return START_TIME;}
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
float Session::updateSession(int newHR) {
    currentHR = newHR;
    elapsedTime++;
    recordedHR.append(newHR);

    if (countUpdates == 5) {
        countUpdates = 0;
        return calculateCoherenceScore();;
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

float Session::calculateCoherenceScore() {
//    currentCoherence = 0; // temp

    // TODO: Figure out how to calculate this shit
    currentCoherence = generateCS();
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

float Session::generateCS() {
    float min = 0.0;
    float max = 7.0;
    float randomFraction = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    float randomNumberInRange = min + (randomFraction * (max - min));
    return randomNumberInRange;
}

