#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QVector>
#include <QDateTime>
#include "session.h"


class DBManager;

class Session;
class Log {
    public:
        Log(
            int sessionNum,
            int profileId,
            int challengeLevel,
            int isLow,
            int isMed,
            int isHigh,
            int sessionTime,
            float achievementScore,
            int coherenceCount,
            QVector<int> heartRates);
        Log(Session*, int);

        int getId();
        int getProfileId();
        int getChallengeLevel();
        int getIsLow();
        int getIsMed();
        int getIsHigh();
        int getSessionTime();
        float getAchievementScore();
        QVector<int> getHeartRates_int();
        QVector<double> getHeartRates_double();
        float getLowCoherencePercentage();
        float getMedCoherencePercentage();
        float getHighCoherencePercentage();
        int getCoherenceCount();

    private:

        int sessionNum;
        int profileId;
        int challengeLevel;
        int isLow;
        int isMed;
        int isHigh;
        int sessionTime;
        float achievementScore;
        int coherenceCount;
        QVector<int> heartRates;
};

#endif
