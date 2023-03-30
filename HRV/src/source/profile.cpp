#include "profile.h"

Profile::Profile(int id, double batteryLvl, int powerLvl) {

    this->id = id;
}


// getters
int Profile::getId() { return id; }
double Profile::getBLvl() { return batteryLvl; }
void Profile::setBLvl(double newLvl) { batteryLvl = newLvl; }
