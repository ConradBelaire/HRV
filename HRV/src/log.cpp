#include "log.h"

// TODO: ADD HRV DATA
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

// TODO: ADD HRV DATA
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

int Log::getId() {return sessionNum;}

int Log::getProfileId() {return profileId;}

int Log::getChallengeLevel() {return challengeLevel;}

int Log::getIsLow() {return isLow;}

int Log::getIsMed() {return isMed;}

int Log::getIsHigh() {return isHigh;}

int Log::getSessionTime() {return sessionTime;}

float Log::getAchievementScore() {return achievementScore;}

float Log::getLowCoherencePercentage(){
    float total_sum = isLow + isMed + isHigh;
    return (isLow / total_sum) * 100;
}

float Log::getMedCoherencePercentage(){
    float total_sum = isLow + isMed + isHigh;
    return (isMed / total_sum) * 100;
}

float Log::getHighCoherencePercentage(){
    float total_sum = isLow + isMed + isHigh;
    return (isHigh / total_sum) * 100;
}

QVector<int> Log::getHeartRates_int() {return heartRates;}
QVector<double> Log::getHeartRates_double() {
    QVector<double> heartRatesDouble;
    for (int value : heartRates) {
        heartRatesDouble.append(static_cast<double>(value));
    }
    return heartRatesDouble;
}

int Log::getCoherenceCount(){return coherenceCount;}
