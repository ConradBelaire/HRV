#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QVector>
#include "session.h"


class DBManager;

class Session;
class Log {
    public:
        // constructors
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
        Log(Session* session, int profileId);

        // getters
        int getId();
        int getIsLow();
        int getIsMed();
        int getIsHigh();
        int getProfileId();
        int getSessionTime();
        int getChallengeLevel();
        int getCoherenceCount();
        float getAchievementScore();
        float getLowCoherencePercentage();
        float getMedCoherencePercentage();
        float getHighCoherencePercentage();
        QVector<int> getHeartRates_int();
        QVector<double> getHeartRates_double();
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
