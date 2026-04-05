// Challenge 1 — Frequency Lock
// The reactor antenna is drifting. Turn the potentiometer until the beeping
// goes solid — that means you've locked onto the right frequency.
// Hold it steady for a few seconds to stabilise. 3 rounds, gets tighter each time.

#pragma once

#include "Challenge.h"
#include <LiquidCrystal_I2C.h>

class FrequencyLock : public Challenge {
public:
  explicit FrequencyLock(LiquidCrystal_I2C& lcd);

  void begin()      override;
  bool update()     override;
  void onGameOver() override;

private:
  enum class State { PLAYING, ROUND_WIN, FAILED };

  LiquidCrystal_I2C& _lcd;
  State         _state;
  uint8_t       _round;
  int           _target;
  unsigned long _roundStartMs;
  unsigned long _inTolStart;
  unsigned long _lastBeep;
  unsigned long _lastLcd;
  unsigned long _pauseStart;

  static const int          ROUND_COUNT       = 3;
  static const int          FREQ_MIN          = 120;
  static const int          FREQ_MAX          = 1200;
  static const unsigned long ROUND_TIMEOUT_MS = 30000UL;
  static const int          TOLERANCE[3];
  static const unsigned long HOLD_MS[3];

  void startRound(uint8_t r);
  int  potToHz(int raw);
  void printLine(uint8_t row, const char* text);
  void successTone();
  void failTone();
};
