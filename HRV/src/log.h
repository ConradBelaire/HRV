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
            int profileId,
            int challengeLevel,
            int isLow,
            int isMed,
            int isHigh,
            float avgCoherence,
            int sessionTime,
            int pacer_duration,
            float achievementScore,
            QString date);
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

    private:

        int sessionNum;
        int profileId;
        int challengeLevel;
        int isLow;
        int isMed;
        int isHigh;
        float avgCoherence;
        int sessionTime;
        int pacerDuration;
        float achievementScore;
        QString date;
};

#endif
