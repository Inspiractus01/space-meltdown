// =============================================================================
// FrequencyLock.cpp
// Space Station Meltdown
// Author: Michal Mucha <michalmucha@proton.me>
// =============================================================================

#include "FrequencyLock.h"
#include "Config.h"

FrequencyLock::FrequencyLock(LiquidCrystal_I2C& lcd)
  : _lcd(lcd)
{}

void FrequencyLock::begin() {
  // TODO
}

bool FrequencyLock::update() {
  // TODO — return true when challenge is solved
  return false;
}

void FrequencyLock::onGameOver() {
  // TODO
}
