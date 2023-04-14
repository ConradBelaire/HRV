#include "profile.h"
/**
 * @brief Profile constructor
 *
 * @param id The profile ID
 * @param batteryLvl The battery level
 * @param sessionAmt The amount of sessions
 */
Profile::Profile(int id, double batteryLvl, int sessionAmt):
    id(id), batteryLvl(batteryLvl), sessionAmt(sessionAmt) {}

// setters
/**
 * @brief Set the battery level
 *
 * @param newLvl The new battery level
 */
void Profile::setBLvl(double newLvl) { batteryLvl = newLvl; }
/**
 * @brief Increase the session amount by one
 *
 * @return The updated session amount
 */
int Profile::increaseSessAmt() { return sessionAmt++; }

// getters
/**
 * @brief Get the profile ID
 *
 * @return The profile ID
 */
int Profile::getId() { return id; }
/**
 * @brief Get the session amount
 *
 * @return The amount of sessions
 */
int Profile::getSessAmt() { return sessionAmt; }
/**
 * @brief Get the battery level
 *
 * @return The battery level
 */
double Profile::getBLvl() { return batteryLvl; }
