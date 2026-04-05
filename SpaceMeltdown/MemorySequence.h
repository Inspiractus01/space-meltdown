// Challenge 2 — Memory Sequence
// The comms array needs recalibrating. Watch the LED pattern and repeat it
// using the buttons. Each round adds one more step and speeds up.

#pragma once

#include "Challenge.h"

class MemorySequence : public Challenge {
public:
  MemorySequence();

  void begin()      override;
  bool update()     override;
  void onGameOver() override;

private:
  // TODO: implement
};
