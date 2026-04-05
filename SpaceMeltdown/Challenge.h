#pragma once

// Base class for all challenges. Each challenge must implement begin() and update().
class Challenge {
public:
  virtual ~Challenge() {}

  // Called once when the challenge starts (reset state, show intro).
  virtual void begin() = 0;

  // Called every loop iteration. Returns true when the challenge is solved.
  virtual bool update() = 0;

  // Called when the player runs out of time.
  virtual void onGameOver() {}
};
