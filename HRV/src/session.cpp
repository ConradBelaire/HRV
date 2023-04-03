#include "session.h"

Session::Session(int session_num, int challenge_level, int pacer_duration, const QDateTime& start_time): SESSION_NUM(session_num), CHALLENGE_LEVEL(challenge_level), PACER_DURATION(pacer_duration), START_TIME(start_time) {
    coherence_sum = 0;
    current_coherence = 0;
    current_heart_rate = 0;
    eleapsed_time = 0;
    achievement_score = 0;
    time_in_high = 0;
    time_in_med = 0;
    time_in_low = 0;
}

// getters
int Session::getSessionNum() const {
    return SESSION_NUM;
}

int Session::getChallengeLevel() const {
    return CHALLENGE_LEVEL;
}

int Session::getPacerDuration() const {
    return PACER_DURATION;
}

float Session::getCoherentSum() const {
    return coherence_sum;
}

float Session::getCurrentCoherence() const {
    return current_coherence;
}

int Session::getCurrentHeartRate() const {
    return current_heart_rate;
}

int Session::getEleapsedTime() const {
    return eleapsed_time;
}

QDateTime Session::getStartTime() const {
    return START_TIME;
}

QTimer* Session::getTimer() {
    return timer;
}

int Session::getTimeLow() const {
    return time_in_low;
}

int Session::getTimeMed() const {
    return time_in_med;
}

int Session::getTimeHigh() const{
    return time_in_high;
}

QVector<int> Session::getGraph() const {
    return recordedHR;
}

// setters
void Session::addToLow() {
    time_in_low++;
}

void Session::addToMed() {
    time_in_med++;
}

void Session::addToHigh() {
    time_in_high++;
}

// functions
void Session::updateReading(int hr) {
    current_heart_rate = hr;
    eleapsed_time += 1;
    recordedHR.append(hr);
}

void Session::addCoherenceScore(float coherence) {
    coherence_sum += coherence;
    coherence_count++;
    achievement_score = coherence_sum / coherence_count;
}
