#include "log.h"

// TODO: ADD HRV DATA
Log::Log(
    int sessionNum,
    int profileId,
    int challengeLevel,
    int isLow,
    int isMed,
    int isHigh,
    float avgCoherence,
    int sessionTime,
    int pacerDuration,
    float achievementScore,
    QString date) :
    sessionNum(sessionNum),
    profileId(profileId),
    challengeLevel(challengeLevel),
    isLow(isLow),
    isMed(isMed),
    isHigh(isHigh),
    avgCoherence(avgCoherence),
    sessionTime(sessionTime),
    pacerDuration(pacerDuration),
    achievementScore(achievementScore),
    date(date) {}

// TODO: ADD HRV DATA
Log::Log(Session* session, int profileId) :
    sessionNum(session->getSessionNum()),
    profileId(profileId),
    challengeLevel(session->getChallengeLevel()),
    isLow(session->getTimeLow()),
    isMed(session->getTimeMed()),
    isHigh(session->getTimeHigh()),
    avgCoherence(session->getAchievementScore() / session->getElapsedTime()),
    sessionTime(session->getElapsedTime()),
    pacerDuration(session->getPacerDuration()),
    achievementScore(session->getAchievementScore()),
    date(session->getStartTime().toString()) {}

int Log::getId() {return sessionNum;}

int Log::getProfileId() {return profileId;}

int Log::getChallengeLevel() {return challengeLevel;}

int Log::getIsLow() {return isLow;}

int Log::getIsMed() {return isMed;}

int Log::getIsHigh() {return isHigh;}

float Log::getAvgCoherence() {return avgCoherence;}

int Log::getPacerDuration() {return pacerDuration;}

int Log::getSessionTime() {return sessionTime;}

float Log::getAchievementScore() {return achievementScore;}

QString Log::getDate() {return date;}

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
