#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QVector>
#include <QDateTime>
#include "session.h"


class DBManager;

class Log {
    public:
        // Log(int, int, int, float, float, float, float, int, float, const QString&, const QString&);
        // Log(int, int, int, float, float, float, float, int, float, QVector<int>, const QDateTime&);
        Log(Session*);

        int getId();
        int getProfileId();
        int getChallengeLevel();
        float getIsLow();
        float getIsMed();
        float getIsHigh();
        float getAvgCoherence();
        int getSessionTime();
        float getAchievementScore();
        const QString& getGraph();
        const QString& getDate();

        void setId(int);
        void setProfileId(int);
        void setChallengeLevel(int);
        void setIsLow(float);
        void setIsMed(float);
        void setIsHigh(float);
        void setAvgCoherence(float);
        void setSessionTime(int);
        void setAchievementScore(float);
        void setGraph(const QString&);
        void setDate(const QString&);
        void setGraph(QVector<int> arr);
        void setDate(const QDateTime& date);

        QString toString();

    private:
        int session_num;
        int profile_id;
        int challenge_level;
        float is_low;
        float is_med;
        float is_high;
        float avg_coherence;
        int session_time;
        float achievement_score;
        QString graph;
        QString date;
};

#endif
