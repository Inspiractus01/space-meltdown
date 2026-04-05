// Challenge 4 — Stabilize the Core
// The reactor core is shaking. Use the potentiometer to keep your reading
// inside the target zone shown on the LEDs. Hold it there for 3 seconds.
// The zone shrinks and drifts faster each round.

#pragma once

#include "Challenge.h"
#include <LiquidCrystal_I2C.h>

class StabilizeCore : public Challenge {
public:
  explicit StabilizeCore(LiquidCrystal_I2C& lcd);

  void begin()      override;
  bool update()     override;
  void onGameOver() override;

private:
  LiquidCrystal_I2C& _lcd;

  // TODO: implement
};
