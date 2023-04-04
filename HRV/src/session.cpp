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
    achievementScore(0),
    timer(timer)   {}

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

float Session::getCoherentSum() const {return coherenceSum;}
float Session::getCurrentCoherence() const {return currentCoherence;}
float Session::getAchievementScore() const {return achievementScore;}

QTimer* Session::getTimer() {return timer;}
QDateTime Session::getStartTime() const {return START_TIME;}
QVector<int> Session::getGraph() const {return recordedHR;}

// setters
void Session::addToLow() {timeInLow++;}
void Session::addToMed() {timeInMed++;}
void Session::addToHigh() {timeInHigh++;}

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
    achievementScore = coherenceSum / coherenceCount;
}

float Session::calculateCoherenceScore() {
    currentCoherence = 0; // temp

    // TODO: Figure out how to calculate this shit

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
