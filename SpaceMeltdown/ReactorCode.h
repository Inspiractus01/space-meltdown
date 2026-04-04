// =============================================================================
// ReactorCode.h — Challenge 3: Reactor Code
// Binary number shown on 7-segment; convert to decimal and enter via buttons.
// 3-bit → 5-bit → 7-bit.
// Space Station Meltdown
// Author: Michal Mucha <michalmucha@proton.me>
// =============================================================================

#pragma once

#include "Challenge.h"
#include <LiquidCrystal_I2C.h>

class ReactorCode : public Challenge {
public:
  explicit ReactorCode(LiquidCrystal_I2C& lcd);

  void begin()    override;
  bool update()   override;
  void onGameOver() override;

private:
  LiquidCrystal_I2C& _lcd;

  // TODO: implement
};
