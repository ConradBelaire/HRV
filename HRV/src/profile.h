#ifndef PROFILE_H
#define PROFILE_H

class Profile {

public:
    Profile(int, double, int);
    int getId();
    double getBLvl();
    void setBLvl(double);
    int getSessAmt();
    void increaseSessAmt() { sessionAmt++; }

private:
    int id;
    double batteryLvl;
    int sessionAmt;
};

#endif // PROFILE_H
