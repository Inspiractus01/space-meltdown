// Challenge 3 — Reactor Code
// The reactor core is locked. A binary number appears on the display —
// convert it to decimal and press the matching button (1-8).
// Gets faster each round. Two wrong answers and the reactor blows.

#pragma once

#include "Challenge.h"
#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>

class ReactorCode : public Challenge {
public:
  ReactorCode(LiquidCrystal_I2C& lcd, TM1638plus& tm);

  void begin()      override;
  bool update()     override;
  void onGameOver() override;

private:
  enum class State { SHOWING, WAITING, CODE_WIN, ROUND_WIN, FAILED };

  LiquidCrystal_I2C& _lcd;
  TM1638plus&        _tm;
  State              _state;

  uint8_t  _round;        // 0-2
  uint8_t  _codesDone;    // codes solved in this round
  uint8_t  _answer;       // correct button index (0-7) = value 1-8
  uint8_t  _lives;        // wrong attempts left
  unsigned long _showStart;
  unsigned long _showDuration;
  unsigned long _pauseStart;

  uint8_t _prevButtons;
  unsigned long _lastBtnRead;

  static const uint8_t ROUNDS = 4;
  static const uint8_t CODES_PER_ROUND[ROUNDS];
  static const unsigned long SHOW_TIME[ROUNDS];

  void     nextCode();
  void     showBinary(uint8_t val);   // display binary on 7-seg
  void     lcdStatus(const char* l0, const char* l1);
  uint8_t  readNewPress();
  void     successTone();
  void     failTone();
};
