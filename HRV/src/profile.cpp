#include "profile.h"

Profile::Profile(int id, double batteryLvl, int sessionAmt) {

    this->id = id;
    this->batteryLvl = batteryLvl;
    this->sessionAmt = sessionAmt;
}


// getters
int Profile::getId() { return id; }
double Profile::getBLvl() { return batteryLvl; }
void Profile::setBLvl(double newLvl) { batteryLvl = newLvl; }
int Profile::getSessAmt() { return sessionAmt; }
void Profile::increaseSessAmt() { sessionAmt++; }