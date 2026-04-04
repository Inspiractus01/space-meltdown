// =============================================================================
// StabilizeCore.h — Challenge 4: Stabilize the Core
// Keep potentiometer position inside a target zone (LED bar graph) for 3 s.
// Zone shrinks and moves faster each round.
// Space Station Meltdown
// Author: Michal Mucha <michalmucha@proton.me>
// =============================================================================

#pragma once

#include "Challenge.h"
#include <LiquidCrystal_I2C.h>

class StabilizeCore : public Challenge {
public:
  explicit StabilizeCore(LiquidCrystal_I2C& lcd);

  void begin()    override;
  bool update()   override;
  void onGameOver() override;

private:
  LiquidCrystal_I2C& _lcd;

  // TODO: implement
};
