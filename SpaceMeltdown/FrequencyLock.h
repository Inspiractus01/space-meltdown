// =============================================================================
// FrequencyLock.h — Challenge 1: Frequency Lock
// Turn the potentiometer to match a target frequency shown on the LCD.
// The buzzer beeps faster the closer you get. 3 rounds, tolerance tightens.
// Space Station Meltdown
// Author: Michal Mucha <michalmucha@proton.me>
// =============================================================================

#pragma once

#include "Challenge.h"
#include <LiquidCrystal_I2C.h>

class FrequencyLock : public Challenge {
public:
  explicit FrequencyLock(LiquidCrystal_I2C& lcd);

  void begin()    override;
  bool update()   override;
  void onGameOver() override;

private:
  LiquidCrystal_I2C& _lcd;

  // TODO: implement
};
