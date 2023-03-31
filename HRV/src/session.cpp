#include "session.h"

Session::Session(int session_num, int challenge_level, int pacer_duration, const QDateTime& start_time): SESSION_NUM(session_num), CHALLENGE_LEVEL(challenge_level), PACER_DURATION(pacer_duration), START_TIME(start_time) {
    coherence_sum = 0;
    current_coherence = 0;
    current_heart_rate = 0;
    eleapsed_time = 0;
    achievement_score = 0;
}

int Session::getSessionNum() const {
    return SESSION_NUM;
}

int Session::getChallengeLevel() const {
    return CHALLENGE_LEVEL;
}

int Session::getPacerDuration() const {
    return pacer_duration;
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

int Session::getElapsedTime() const {
    return eleapsed_time;
}

QDateTime Session::getStartTime() const {
    return START_TIME;
}

// functions
void Session::updateReading(int hr) {
    current_heart_rate = hr;
    eleapsed_time += 1;
}

void Session::addCoherenceScore(float coherence) {
    coherence_sum += coherence;
    coherence_count++;
    achievement_score = coherence_sum / coherence_count;
}
