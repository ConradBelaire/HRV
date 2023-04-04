#ifndef SESSION_H
#define SESSION_H

#include <QDateTime>
#include <QTimer>
#include <QVector>

class Session {

    public:
        Session(
            int sessionNum,
            int challengeLevel,
            int pacerDuration,
            const QDateTime& startTime,
            QTimer* timer
        );

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

        float getCoherentSum() const;
        float getCurrentCoherence() const;
        float getAchievementScore() const;

        QTimer* getTimer();
        QDateTime getStartTime() const;
        QVector<int> getGraph() const;

        // setters
        void addToLow();
        void addToMed();
        void addToHigh();

        // functions
        float updateSession(int newHR);
        int determineScoreLevel(float cohernceScore);



    private:
        const int SESSION_NUM;
        const int CHALLENGE_LEVEL;
        const int PACER_DURATION;
        const QDateTime START_TIME;\

        int currentHR;
        int timeInLow;
        int timeInMed;
        int timeInHigh;
        int elapsedTime;
        int countUpdates;
        int coherenceCount;

        float coherenceSum;
        float currentCoherence;
        float achievementScore;


        QTimer* timer;
        QVector<int> recordedHR;

        void addCoherenceScore(float newCoherenceScore);
        float calculateCoherenceScore();
};

#endif // SESSION_H
