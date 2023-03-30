#include "log.h"

Log::Log(int sessionNum, int pid, int challengeLevel, float isLow, float isMed, float isHigh, float avgCoherence, int sessionTime, float achievementScore, const QString& graph, const QString& date) {
    this->session_num = sessionNum;
    this->profile_id = pid;
    this->challenge_level = challengeLevel;
    this->is_low = isLow;
    this->is_med = isMed;
    this->is_high = isHigh;
    this->avg_coherence = avgCoherence;
    this->session_time = sessionTime;
    this->achievement_score = achievementScore;

    this->graph = graph;
    this->date = date;
}

Log::Log(int sessionNum, int pid, int challengeLevel, float isLow, float isMed, float isHigh, float avgCoherence, int sessionTime, float achievementScore, QVector<int> HRarr, const QDateTime& date) {
    this->session_num = sessionNum;
    this->profile_id = pid;
    this->challenge_level = challengeLevel;
    this->is_low = isLow;
    this->is_med = isMed;
    this->is_high = isHigh;
    this->avg_coherence = avgCoherence;
    this->session_time = sessionTime;
    this->achievement_score = achievementScore;

    // format date
    this->date = date.toString("dd.MM.yyyy hh:mm:ss");
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

const QString& Log::getGraph() {
    return graph;
}

const QString& Log::getDate() {
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
