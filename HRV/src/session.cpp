#include "session.h"

Session::Session(int sessionNum, int pid, int challengeLevel, float isLow, float isMed, float isHigh, float avgCoherence, int sessionTime, float achievementScore, const QString& graph, const QString& date) {
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

Session::Session(int sessionNum, int pid, int challengeLevel, float isLow, float isMed, float isHigh, float avgCoherence, int sessionTime, float achievementScore, QVector<int> HRarr, const QDateTime& date) {
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
    this->date = date.toString("dd.MM.yyyy - hh:mm:ss");
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

int Session::getId() {
    return session_num;
}

int Session::getProfileId() {
    return profile_id;
}

int Session::getChallengeLevel() {
    return challenge_level;
}

float Session::getIsLow() {
    return is_low;
}

float Session::getIsMed() {
    return is_med;
}

float Session::getIsHigh() {
    return is_high;
}

float Session::getAvgCoherence() {
    return avg_coherence;
}

int Session::getSessionTime() {
    return session_time;
}

float Session::getAchievementScore() {
    return achievement_score;
}

const QString& Session::getGraph() {
    return graph;
}

const QString& Session::getDate() {
    return date;
}



void Session::setId(int id) {
    this->session_num = id;
}

void Session::setProfileId(int profile_id) {
    this->profile_id = profile_id;
}

void Session::setChallengeLevel(int challenge_level) {
    this->challenge_level = challenge_level;
}

void Session::setIsLow(float is_low) {
    this->is_low = is_low;
}

void Session::setIsMed(float is_med) {
    this->is_med = is_med;
}

void Session::setIsHigh(float is_high) {
    this->is_high = is_high;
}

void Session::setAvgCoherence(float avg_coherence) {
    this->avg_coherence = avg_coherence;
}

void Session::setSessionTime(int session_time) {
    this->session_time = session_time;
}

void Session::setAchievementScore(float achievement_score) {
    this->achievement_score = achievement_score;
}

void Session::setGraph(const QString& graph) {
    this->graph = graph;
}

void Session::setDate(const QString& date) {
    this->date = date;
}


void Session::setGraph(QVector<int> HRarr) {
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

void Session::setDate(const QDateTime& date) {
    this->date = date.toString("dd.MM.yyyy - hh:mm:ss");
}
