#ifndef SESSION_H
#define SESSION_H

#include <QDateTime>
#include <QTimer>

class Session {

    public:
        Session(int session_num, int challenge_level, int pacer_duration, const QDateTime& start_time);

        // getters
        int getChallengeLevel() const;
        int getSessionNum() const;
        int getPacerDuration() const;
        float getCoherentSum() const;
        int getCoherenceCount() const;
        float getCurrentCoherence() const;
        int getCurrentHeartRate() const;
        int getEleapsedTime() const;
        float getAchievementScore() const;
        QDateTime getStartTime() const;
        QTimer* getTimer();

        // functions
        void updateReading(int hr);
        void addCoherenceScore(float coherence);

    private:
        const int CHALLENGE_LEVEL;
        const int SESSION_NUM;
        const QDateTime START_TIME;
        const int PACER_DURATION;

        QTimer* timer;
        float coherence_sum;
        int coherence_count;
        float current_coherence;
        int current_heart_rate;
        int eleapsed_time;
        float achievement_score;
};

#endif // PROFILE_H
