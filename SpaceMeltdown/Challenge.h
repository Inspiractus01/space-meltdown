// =============================================================================
// Challenge.h — Abstract base class for all escape room challenges
// Space Station Meltdown
// Author: Michal Mucha <michalmucha@proton.me>
// =============================================================================

#pragma once

// Every challenge must implement this interface.
class Challenge {
public:
  virtual ~Challenge() {}

  // Called once when the challenge starts (reset state, show intro).
  virtual void begin() = 0;

  // Called every loop iteration. Returns true when the challenge is solved.
  virtual bool update() = 0;

  // Called when the player fails the whole game (show failure animation, etc.).
  virtual void onGameOver() {}
};
