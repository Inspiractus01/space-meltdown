// Challenge 2 — Memory Sequence
// The comms array needs recalibrating. Watch which LEDs light up, then
// repeat the pattern using the buttons. Each round adds one more step
// and speeds up. One wrong press and you fail.

#pragma once

#include "Challenge.h"
#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>

class MemorySequence : public Challenge {
public:
  MemorySequence(LiquidCrystal_I2C& lcd, TM1638plus& tm);

  void begin()      override;
  bool update()     override;
  void onGameOver() override;

private:
  enum class State { SHOWING_ON, SHOWING_OFF, PLAYER_TURN, ROUND_WIN, FAILED };

  LiquidCrystal_I2C& _lcd;
  TM1638plus&        _tm;
  State              _state;

  uint8_t  _sequence[16];  // random LED indices (0-7)
  uint8_t  _seqLen;        // current sequence length
  uint8_t  _showIdx;       // which step we are showing
  uint8_t  _inputIdx;      // which step we are waiting for from player

  unsigned long _stateMs;      // when current state started
  unsigned long _showOnMs;     // how long LED stays on  (shrinks each round)
  unsigned long _showOffMs;    // gap between LEDs       (shrinks each round)
  unsigned long _pauseStart;   // for ROUND_WIN pause

  uint8_t _prevButtons;        // for edge detection / debounce
  unsigned long _lastBtnRead;

  static const uint8_t  ROUNDS      = 5;   // total rounds
  static const uint8_t  START_LEN   = 3;   // sequence length for round 1

  void     startRound(uint8_t round);
  void     showStep(uint8_t idx);    // display sequence step on 7-seg
  void     lcdStatus(const char* line0, const char* line1);
  uint8_t  readNewPress();
  void     successTone();
  void     failTone();
};
