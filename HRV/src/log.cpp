#include "log.h"

// TODO: ADD HRV DATA
Log::Log(
    int sessionNum,
    int profile_id,
    int challengeLevel,
    int isLow,
    int isMed,
    int isHigh,
    float avgCoherence,
    int sessionTime,
    int pacer_duration;
    float achievementScore,
    QString date,
):
    session_num(sessionNum),
    profile_id(profile_id),
    challenge_level(challengeLevel),
    pacer_duration(pacer_duration),
    avg_coherence(avgCoherence),
    is_low(isLow),
    is_med(isMed),
    is_high(isHigh),
    session_time(sessionTime),
    achievement_score(achievementScore),
    date(date) {}

// TODO: ADD HRV DATA
Log::Log(Session* session, int profile_id):
    session_num(session->getSessionNum()),
    profile_id(profile_id),
    challenge_level(session->getChallengeLevel()),
    pacer_duration(session->getPacerDuration()),
    avg_coherence(session->getCoherentSum() / session->getEleapsedTime()),
    is_low(session->getTimeLow()),
    is_med(session->getTimeMed()),
    is_high(session->getTimeHigh()),
    session_time(session->getEleapsedTime()),
    achievement_score(session->getAchievementScore()),
    date(session->getStartTime().toString()) {}

int Log::getId() {
    return session_num;
}

int Log::getProfileId() {
    return profile_id;
}

int Log::getChallengeLevel() {
    return challenge_level;
}

float Log::getIsLow() {
    return is_low;
}

float Log::getIsMed() {
    return is_med;
}

float Log::getIsHigh() {
    return is_high;
}

float Log::getAvgCoherence() {
    return avg_coherence;
}

int Log::getSessionTime() {
    return session_time;
}

float Log::getAchievementScore() {
    return achievement_score;
}

int Log::getPacerDuration() {
    return pacer_duration;
}

const QString& Log::getGraph() {
    return graph;
}

QDateTime Log::getDate() {
    return date;
}



void Log::setId(int id) {
    this->session_num = id;
}

void Log::setProfileId(int profile_id) {
    this->profile_id = profile_id;
}

void Log::setChallengeLevel(int challenge_level) {
    this->challenge_level = challenge_level;
}

void Log::setIsLow(float is_low) {
    this->is_low = is_low;
}

void Log::setIsMed(float is_med) {
    this->is_med = is_med;
}

void Log::setIsHigh(float is_high) {
    this->is_high = is_high;
}

void Log::setAvgCoherence(float avg_coherence) {
    this->avg_coherence = avg_coherence;
}

void Log::setSessionTime(int session_time) {
    this->session_time = session_time;
}

void Log::setAchievementScore(float achievement_score) {
    this->achievement_score = achievement_score;
}

void Log::setGraph(const QString& graph) {
    this->graph = graph;
}

void Log::setDate(const QString& date) {
    this->date = date;
}


void Log::setGraph(QVector<int> HRarr) {
    // convert vector to string
    if (HRarr.size()>1) {
        QString data = QString::number(HRarr[0]);
        for (int i = 1; i < HRarr.size(); i++) {
            data.append(",");
            data.append(QString::number(HRarr[i]));
        }
    } else {
        this->graph = QString::number(HRarr[0]);
    }
}

void Log::setDate(const QDateTime& date) {
    this->date = date.toString("dd.MM.yyyy hh:mm:ss");
}

QString Log::toString() {
    QString str = "Session ID: " + QString::number(session_num) + "\n Date: " + date + "Challenge Lvl: " + challenge_level;
    return str;
}


float Log::getLowCoherencePercentage(){
    float total_sum = is_low + is_med + is_high;
    return (is_low / total_sum) * 100;
}

float Log::getMedCoherencePercentage(){
    float total_sum = is_low + is_med + is_high;
    return (is_med / total_sum) * 100;
}

float Log::getHighCoherencePercentage(){
    float total_sum = is_low + is_med + is_high;
    return (is_high / total_sum) * 100;
}
