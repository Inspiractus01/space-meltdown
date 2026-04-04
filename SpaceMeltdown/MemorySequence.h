// =============================================================================
// MemorySequence.h — Challenge 2: Memory Sequence (Simon Says)
// LEDs flash a sequence; repeat it using the buttons. Gets longer and faster.
// Space Station Meltdown
// Author: Michal Mucha <michalmucha@proton.me>
// =============================================================================

#pragma once

#include "Challenge.h"

class MemorySequence : public Challenge {
public:
  MemorySequence();

  void begin()    override;
  bool update()   override;
  void onGameOver() override;

private:
  // TODO: implement
};
