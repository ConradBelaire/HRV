#ifndef SESSION_H
#define SESSION_H

#include <QTimer>
#include <QVector>
#include <QDebug>
#include "log.h"

class Log;

class Session {

    public:
        Session(
            int sessionNum,
            int challengeLevel,
            int pacerDuration,
            QTimer* timer
        );

        Session(Log* log);

        // getters
        int getTimeLow() const;
        int getTimeMed() const;
        int getTimeHigh() const;
        int getSessionNum() const;
        int getElapsedTime() const;
        int getPacerDuration() const;
        int getChallengeLevel() const;
        int getCoherenceCount() const;
        int getCurrentHeartRate() const;

        float getCurrentCoherence() const;
        float getAchievementScore() const;

        QTimer* getTimer();
        QVector<double> getGraph_double() const;
        QVector<int> getGraph_int() const;

        // setters
        void addToLow();
        void addToMed();
        void addToHigh();

        void setAvgCoherence(float newAvgCoherence);
        void setAchievementScore(float newAchievementScore);
        void setLowCoherencePercentage(float newLowCoherencePercentage);
        void setMedCoherencePercentage(float newMedCoherencePercentage);
        void setHighCoherencePercentage(float newHighCoherencePercentage);
        void setHeartRates_double(QVector<double> newHeartRates_double);

        // functions
        float updateSession(int newHR, int currentVector);
        int determineScoreLevel(float cohernceScore);

    private:
        const int SESSION_NUM;
        const int CHALLENGE_LEVEL;
        const int PACER_DURATION;

        int currentHR;
        int timeInLow;
        int timeInMed;
        int timeInHigh;
        int elapsedTime;
        int countUpdates;
        int coherenceCount;
        float newAvgCoherence;

        float coherenceSum;
        float currentCoherence;

        QTimer* timer;
        QVector<double> recordedHR;

        void addCoherenceScore(float newCoherenceScore);
        float calculateCoherenceScore(int currentVector);

        QVector<double> coherenceScoreData;
};

#endif // SESSION_H
