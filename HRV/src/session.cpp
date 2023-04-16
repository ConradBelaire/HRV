#include "session.h"


/**
 * @brief Constructor for Session class with session parameters as arguments.
 * @param sessionNum: The session number.
 * @param challengeLevel: The challenge level for the session.
 * @param pacerDuration: The pacer duration for the session.
 * @param timer: A QTimer object pointer for the session timer.
 */
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

/**
 * @brief Constructor for Session class that takes a Log object as argument.
 * @param log: A Log object pointer containing data to initialize a Session object.
 */
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

/**
 * @brief Returns the time spent in the low HR zone.
 * @return The time spent in the low HR zone as an int.
 */
int Session::getTimeLow() const {return timeInLow;}

/**
 * @brief Returns the time spent in the medium HR zone.
 * @return The time spent in the medium HR zone as an int.
 */
int Session::getTimeMed() const {return timeInMed;}

/**
 * @brief Returns the time spent in the high HR zone.
 * @return The time spent in the high HR zone as an int.
 */
int Session::getTimeHigh() const {return timeInHigh;}

/**
 * @brief Returns the session number.
 * @return The session number as an int.
 */
int Session::getSessionNum() const {return SESSION_NUM;}

/**
 * @briefReturns the elapsed time for the session.
 * @return The elapsed time as an int.
 */
int Session::getElapsedTime() const {return elapsedTime;}

/**
 * @brief Returns the pacer duration for the session.
 * @return The pacer duration as an int.
 */
int Session::getPacerDuration() const {return PACER_DURATION;}

/**
 * @brief Returns the challenge level for the session.
 * @return The challenge level as an int.
 */
int Session::getChallengeLevel() const {return CHALLENGE_LEVEL;}

/**
 * @brief Returns the coherence count for the session.
 * @return The coherence count as an int.
 */
int Session::getCoherenceCount() const {return coherenceCount;}

/**
 * @brief Returns the current heart rate.
 * @return The current heart rate as an int.
 */
int Session::getCurrentHeartRate() const {return currentHR;}

/**
 * @brief Returns the current coherence score.
 * @return The current coherence score as a float.
 */
float Session::getCurrentCoherence() const {return currentCoherence;}

/**
 * @brief Returns the achievement score for the session.
 * @return The achievement score as a float.
 */
float Session::getAchievementScore() const {return coherenceSum;}

/**
 * @brief Returns a pointer to the QTimer object.
 * @return A pointer to the QTimer object.
 */
QTimer* Session::getTimer() {return timer;}

/**
 * @brief Returns a QVector of double containing recorded heart rates.
 * @return A QVector of double with recorded heart rates.
 */
QVector<double> Session::getGraph_double() const {return recordedHR;}

/**
 * @brief Returns a QVector of int containing recorded heart rates.
 * @return A QVector of int with recorded heart rates.
 */
QVector<int> Session::getGraph_int() const {
    QVector<int> list_of_ints;
    for (double value : recordedHR) {
        list_of_ints.append(static_cast<int>(value));
    }
    return list_of_ints;
}

/**
 * @brief Increments the time spent in the low coherence zone and emits a beep if it's the first time.
 */
void Session::addToLow() {
    timeInLow++;
    if (timeInLow == 1) {
        qDebug() << "*BEEP* (low coherence light turned on for the first time)";
    }
}

/**
 * @brief Increments the time spent in the medium coherence zone and emits a beep if it's the first time.
 */
void Session::addToMed() {
    timeInMed++;
    if (timeInMed == 1) {
        qDebug() << "*BEEP* (medium coherence light turned on for the first time)";
    }
}

/**
 * @brief Increments the time spent in the high coherence zone and emits a beep if it's the first time.
 */
void Session::addToHigh() {
    timeInHigh++;
    if (timeInHigh == 1) {
        qDebug() << "*BEEP* (high coherence light turned on for the first time)";
    }
}

/**
 * @brief Sets the achievement score for the session.
 * @param newAchievementScore: The new achievement score as a float.
 */
void Session::setAchievementScore(float newAchievementScore) {coherenceSum = newAchievementScore;}

/**
 * @brief Sets the low coherence percentage for the session.
 * @param newLowCoherencePercentage: The new low coherence percentage as a float.
 */
void Session::setLowCoherencePercentage(float newLowCoherencePercentage) {timeInLow = newLowCoherencePercentage;}

/**
 * @brief Sets the medium coherence percentage for the session.
 * @param newMedCoherencePercentage: The new medium coherence percentage as a float.
 */
void Session::setMedCoherencePercentage(float newMedCoherencePercentage) {timeInMed = newMedCoherencePercentage;}

/**
 * @brief Sets the high coherence percentage for the session.
 * @param newHighCoherencePercentage: The new high coherence percentage as a float.
 */
void Session::setHighCoherencePercentage(float newHighCoherencePercentage) {timeInHigh = newHighCoherencePercentage;}

/**
 * @brief Sets the QVector of recorded heart rates in the session as doubles.
 * @param newHeartRates_double: The new QVector of doubles with recorded heart rates.
 */
void Session::setHeartRates_double(QVector<double> newHeartRates_double) {recordedHR = newHeartRates_double;}

/**
 * @brief Updates the session with a new heart rate value and the coherence score using current vector. coherence score is calculated every 5 seconds.
 * @param newHR: The new heart rate value as an int.
 * @param currentVector: The index of the current vector as an int.
 * @return The calculated coherence score if 5 seconds has passed, otherwise -1.
 */
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

/**
 * @brief Determines the score level based on the coherence score and challenge level.
 * @param cohernceScore: The coherence score to determine the score level as a float.
 * @return The score level (0 for low, 1 for medium, 2 for high) or -1 for an invalid score.
 */
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

/**
 * @brief Adds a new coherence score to the sum and increments the coherence count.
 * @param newCoherenceScore: The new coherence score to be added as a float.
 */
void Session::addCoherenceScore(float newCoherenceScore) {
    coherenceSum += newCoherenceScore;
    coherenceCount++;
}

/**
 * @brief Calculates the coherence score based on the current vector.
 * @param currentVector: The index of the current vector as an int.
 * @return The calculated coherence score as a float.
 */
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
