#ifndef PROFILE_H
#define PROFILE_H

class DBManager;

class Profile {

public:
    Profile(int, double);
    int getId();
    double getBLvl();
    void setBLvl(double);

private:
    int id;

};

#endif // PROFILE_H
