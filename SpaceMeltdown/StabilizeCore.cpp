// =============================================================================
// StabilizeCore.cpp
// Space Station Meltdown
// Author: Michal Mucha <michalmucha@proton.me>
// =============================================================================

#include "StabilizeCore.h"
#include "Config.h"

StabilizeCore::StabilizeCore(LiquidCrystal_I2C& lcd)
  : _lcd(lcd)
{}

void StabilizeCore::begin() {
  // TODO
}

bool StabilizeCore::update() {
  // TODO — return true when challenge is solved
  return false;
}

void StabilizeCore::onGameOver() {
  // TODO
}
