#include "profile.h"

Profile::Profile(int id, double batteryLvl) {

    this->id = id;
    this->batteryLvl = batteryLvl;
}


// getters
int Profile::getId() { return id; }
double Profile::getBLvl() { return batteryLvl; }
void Profile::setBLvl(double newLvl) { batteryLvl = newLvl; }
