#include "profile.h"
/**
 * Profile constructor
 *
 * @param id The profile ID
 * @param batteryLvl The battery level
 * @param sessionAmt The amount of sessions
 */
Profile::Profile(int id, double batteryLvl, int sessionAmt):
    id(id), batteryLvl(batteryLvl), sessionAmt(sessionAmt) {}

// setters
/**
 * Set the battery level
 *
 * @param newLvl The new battery level
 */
void Profile::setBLvl(double newLvl) { batteryLvl = newLvl; }
/**
 * Increase the session amount by one
 *
 * @return The updated session amount
 */
int Profile::increaseSessAmt() { return sessionAmt++; }

// getters
/**
 * Get the profile ID
 *
 * @return The profile ID
 */
int Profile::getId() { return id; }
/**
 * Get the session amount
 *
 * @return The amount of sessions
 */
int Profile::getSessAmt() { return sessionAmt; }
/**
 * Get the battery level
 *
 * @return The battery level
 */
double Profile::getBLvl() { return batteryLvl; }
