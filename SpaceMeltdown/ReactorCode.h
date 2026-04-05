// Challenge 3 — Reactor Code
// The reactor core is locked behind a binary access code. Read the number
// shown on the 7-segment display, convert it to decimal and enter it using
// the buttons. Codes get longer: 3-bit → 5-bit → 7-bit.

#pragma once

#include "Challenge.h"
#include <LiquidCrystal_I2C.h>

class ReactorCode : public Challenge {
public:
  explicit ReactorCode(LiquidCrystal_I2C& lcd);

  void begin()      override;
  bool update()     override;
  void onGameOver() override;

private:
  LiquidCrystal_I2C& _lcd;

  // TODO: implement
};
