#ifndef PROFILE_H
#define PROFILE_H

class Profile {

public:
    // constructors
    Profile(int id, double batteryLvl, int sessionAmt);
    
    // setters
    void setBLvl(double);
    int increaseSessAmt();

    // getters
    int getId();
    int getSessAmt();
    double getBLvl();

private:
    int id;
    double batteryLvl;
    int sessionAmt;
};

#endif // PROFILE_H
