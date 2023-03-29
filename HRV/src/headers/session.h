#ifndef SESSION_H
#define SESSION_H

#include <string>

class DBManager;

class Session {
    public:
        Session(int, int, int, float, float, float, float, int, float, string, string);

        int getId();
        int getProfileId();
        int getChallengeLevel();
        float getIsLow();
        float getIsMed();
        float getIsHigh();
        float getAvgCoherence();
        int getSessionTime();
        float getAchievementScore();
        string getGraph();
        string getDate();

        void setId(int);
        void setProfileId(int);
        void setChallengeLevel(int);
        void setIsLow(float);
        void setIsMed(float);
        void setIsHigh(float);
        void setAvgCoherence(float);
        void setSessionTime(int);
        void setAchievementScore(float);
        void setGraph(string);
        void setDate(string);

    private:
        int id;
        int profile_id;
        int challenge_level;
        float is_low;
        float is_med;
        float is_high;
        float avg_coherence;
        int session_time;
        float achievement_score;
        string graph;
        string date;
}

#endif
