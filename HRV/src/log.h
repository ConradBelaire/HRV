#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QVector>
#include <QDateTime>
#include "session.h"


class DBManager;

class Log {
    public:
        Log(
            int sessionNum,
            int profile_id,
            int challengeLevel,
            int pacer_duration,
            float avgCoherence,
            int isLow,
            int isMed,
            int isHigh,
            int sessionTime,
            float achievementScore,
            QString date,
        );
        Log(Session*, int);

        int getId();
        int getProfileId();
        int getChallengeLevel();
        int getIsLow();
        int getIsMed();
        int getIsHigh();
        float getAvgCoherence();
        int getSessionTime();
        int getPacerDuration();
        float getAchievementScore();
        QString getDate();
        float getLowCoherencePercentage();
        float getMedCoherencePercentage();
        float getHighCoherencePercentage();

        void setId(int);
        void setProfileId(int);
        void setChallengeLevel(int);
        void setIsLow(float);
        void setIsMed(float);
        void setIsHigh(float);
        void setAvgCoherence(float);
        void setSessionTime(int);
        void setAchievementScore(float);
        void setDate(const QString&);
        void setDate(const QDateTime& date);

        QString toString();

    private:
        int session_num;
        int profile_id;
        int challenge_level;
        int pacer_duration;
        float avg_coherence;
        int is_low;
        int is_med;
        int is_high;

        int session_time;
        float achievement_score;

        QString date;
};

#endif
