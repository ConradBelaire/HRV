#include "profile.h"

Profile::Profile(int id, double batteryLvl, int sessionAmt):
    id(id), batteryLvl(batteryLvl), sessionAmt(sessionAmt) {}

// getters
int Profile::getId() { return id; }
double Profile::getBLvl() { return batteryLvl; }
void Profile::setBLvl(double newLvl) { batteryLvl = newLvl; }
int Profile::getSessAmt() { return sessionAmt; }
int Profile::increaseSessAmt() { return sessionAmt++; }
