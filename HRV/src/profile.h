#ifndef PROFILE_H
#define PROFILE_H

class Profile {

public:
    Profile(int, double);
    int getId();
    double getBLvl();
    void setBLvl(double);

private:
    int id;
    double batteryLvl;
};

#endif // PROFILE_H
