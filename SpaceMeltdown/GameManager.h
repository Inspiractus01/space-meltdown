// =============================================================================
// GameManager.h — Orchestrates the overall game flow
// Space Station Meltdown
// Author: Michal Mucha <michalmucha@proton.me>
// =============================================================================

#pragma once

#include "Challenge.h"
#include "Config.h"

enum class GameState {
  INTRO,        // Welcome screen, waiting for player to start
  RUNNING,      // A challenge is active
  TRANSITION,   // Brief pause between challenges
  WIN,          // All challenges solved
  GAME_OVER     // Timer ran out
};

class GameManager {
public:
  GameManager();

  // Call once in Arduino setup().
  void begin();

  // Call every Arduino loop() iteration.
  void update();

private:
  GameState     _state;
  int           _currentIndex;     // which challenge is active (0-based)
  unsigned long _gameStartMs;      // millis() when game started
  unsigned long _transitionStartMs;

  // Pointers to all challenges — populated in constructor.
  Challenge*    _challenges[TOTAL_CHALLENGES];

  // --- helpers ---
  int  timeRemainingSeconds() const;
  void startChallenge(int index);
  void showHUD() const;            // update LCD with time + current challenge
  void playWin();
  void playGameOver();
  void showIntro();
  void showTransition(int completedIndex);
};
