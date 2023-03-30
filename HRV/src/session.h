#ifndef SESSION_H
#define SESSION_H

class Session {

    public:
        Session(int, int, int, float, float, float, float, int, float, const QString&, const QString&);
        Session(int, int, int, float, float, float, float, int, float, QVector<int>, const QDateTime&);

        int getId();
        int getProfileId();
        int getChallengeLevel();
        float getIsLow();
        float getIsMed();
        float getIsHigh();
        float getAvgCoherence();
        int getLogTime();
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
        void setLogTime(int);
        void setAchievementScore(float);
        void setGraph(const QString&);
        void setDate(const QString&);
        void setGraph(QVector<int> arr);
        void setDate(const QDateTime& date);




        void update_reading(int hrr);
    private:
        int heart_rate_reading;




        int seesion_num;
        int eleapsed_time;
        float coherent_sum;
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

#endif // PROFILE_H
