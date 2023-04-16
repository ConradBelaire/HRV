#include "log.h"


Log::Log(
    int sessionNum,
    int profileId,
    int challengeLevel,
    int isLow,
    int isMed,
    int isHigh,
    int sessionTime,
    float achievementScore,
    int coherenceCount,
    QVector<int> heartRates) :
    sessionNum(sessionNum),
    profileId(profileId),
    challengeLevel(challengeLevel),
    isLow(isLow),
    isMed(isMed),
    isHigh(isHigh),
    sessionTime(sessionTime),
    achievementScore(achievementScore),
    coherenceCount(coherenceCount),
    heartRates(heartRates) {}

Log::Log(Session* session, int profileId) :
    sessionNum(session->getSessionNum()),
    profileId(profileId),
    challengeLevel(session->getChallengeLevel()),
    isLow(session->getTimeLow()),
    isMed(session->getTimeMed()),
    isHigh(session->getTimeHigh()),
    sessionTime(session->getElapsedTime()),
    achievementScore(session->getAchievementScore()),
    coherenceCount(session->getCoherenceCount()),
    heartRates(session->getGraph_int()) {}

/**
 * @brief Get the session ID
 * @return sessionNum as an integer
*/
int Log::getId() {return sessionNum;}

/**
 * @brief Get the number of low coherence events
 * @return isLow as an integer
*/
int Log::getIsLow() {return isLow;}

/**
 * @brief Get the number of medium coherence events
 * @return isMed as an integer
*/
int Log::getIsMed() {return isMed;}

/**
 * @brief Get the number of high coherence events
 * @return isHigh as an integer
*/
int Log::getIsHigh() {return isHigh;}

/**
 * @brief Get the associated profile ID
 * @return profileId as an integer
*/
int Log::getProfileId() {return profileId;}

/**
 * @brief Get the session duration
 * @return sessionTime as an integer
*/
int Log::getSessionTime() {return sessionTime;}

/**
 * @brief Get the challenge level
 * @return challengeLevel as an integer
*/
int Log::getChallengeLevel() {return challengeLevel;}

/**
 * @brief Get the coherence count
 * @return coherenceCount as an integer
*/
int Log::getCoherenceCount(){return coherenceCount;}

/**
 * @brief Get the achievement score
 * @return achievementScore as a float
*/
float Log::getAchievementScore() {return achievementScore;}

/**
 * @brief Get the low coherence percentage
 * @return low coherence percentage as a float
*/
float Log::getLowCoherencePercentage(){
    float total_sum = isLow + isMed + isHigh;
    return (isLow / total_sum) * 100;
}

/**
 * @brief Get the medium coherence percentage
 * @return medium coherence percentage as a float
*/
float Log::getMedCoherencePercentage(){
    float total_sum = isLow + isMed + isHigh;
    return (isMed / total_sum) * 100;
}

/**
 * @brief Get the high coherence percentage
 * @return high coherence percentage as a float
*/
float Log::getHighCoherencePercentage(){
    float total_sum = isLow + isMed + isHigh;
    return (isHigh / total_sum) * 100;
}

/**
 * @brief Get heart rates as integers
 * @return heartRates as QVector<int>
*/
QVector<int> Log::getHeartRates_int() {return heartRates;}

/**
 * @brief Get heart rates as doubles
 * @return heartRates as QVector<double>
*/
QVector<double> Log::getHeartRates_double() {
QVector<double> heartRatesDouble;
    for (int value : heartRates) {
        heartRatesDouble.append(static_cast<double>(value));
    }
    return heartRatesDouble;
}
