// =============================================================================
// FrequencyLock.cpp — Challenge 1: Frequency Lock
// Space Station Meltdown
// Author: Michal Mucha <michalmucha@proton.me>
// =============================================================================

#include "FrequencyLock.h"
#include "Config.h"

const int          FrequencyLock::TOLERANCE[3] = { 50,   30,   15   };
const unsigned long FrequencyLock::HOLD_MS[3]  = { 1200, 1700, 2300 };

// ---------------------------------------------------------------------------

FrequencyLock::FrequencyLock(LiquidCrystal_I2C& lcd)
  : _lcd(lcd),
    _state(State::PLAYING),
    _round(0), _target(0),
    _roundStartMs(0), _inTolStart(0),
    _lastBeep(0), _lastLcd(0), _pauseStart(0)
{}

// ---------------------------------------------------------------------------

void FrequencyLock::begin() {
  randomSeed(analogRead(A1));
  _round = 0;
  printLine(0, "Frequency Lock");
  printLine(1, "System 1");
  delay(1000);
  startRound(0);
}

// ---------------------------------------------------------------------------

bool FrequencyLock::update() {
  unsigned long now = millis();

  switch (_state) {

    case State::ROUND_WIN:
      if (now - _pauseStart >= 700) {
        _round++;
        if (_round >= ROUND_COUNT) return true;
        startRound(_round);
      }
      return false;

    case State::FAILED:
      return false;

    case State::PLAYING: {
      if (now - _roundStartMs >= ROUND_TIMEOUT_MS) {
        printLine(0, "TIMEOUT");
        printLine(1, "Press RESET");
        failTone();
        _state = State::FAILED;
        return false;
      }

      int current = potToHz(analogRead(POT_PIN));
      int diff    = abs(current - _target);
      int d       = constrain(diff, 0, 500);

      unsigned long interval = map(d, 500, 0, 700, 80);
      int beepFreq           = map(d, 500, 0, 220, 1900);

      if (now - _lastBeep >= interval) {
        tone(BUZZER_PIN, beepFreq, 28);
        _lastBeep = now;
      }

      bool inTol = (diff <= TOLERANCE[_round]);
      if (inTol) {
        if (_inTolStart == 0) _inTolStart = now;
        if (now - _inTolStart >= HOLD_MS[_round]) {
          char buf[17];
          snprintf(buf, sizeof(buf), "Round %d OK!", _round + 1);
          printLine(0, buf);
          printLine(1, "System stable");
          successTone();
          _pauseStart = millis();
          _state = State::ROUND_WIN;
          return false;
        }
      } else {
        _inTolStart = 0;
      }

      if (now - _lastLcd >= 120) {
        _lastLcd = now;
        char buf[17];
        snprintf(buf, sizeof(buf), "R%d T:%d", _round + 1, _target);
        printLine(0, buf);
        int pct = (_inTolStart != 0)
          ? (int)(((now - _inTolStart) * 100UL) / HOLD_MS[_round])
          : 0;
        if (pct > 99) pct = 99;
        snprintf(buf, sizeof(buf), "N%d D%d %d%%", current, diff, pct);
        printLine(1, buf);
      }
      return false;
    }

    default: return false;
  }
}

// ---------------------------------------------------------------------------

void FrequencyLock::onGameOver() {
  printLine(0, "Freq Lock FAIL");
  printLine(1, "");
}

// ---------------------------------------------------------------------------

void FrequencyLock::startRound(uint8_t r) {
  _target       = random(220, 1101);
  _roundStartMs = millis();
  _inTolStart   = 0;
  _lastBeep     = 0;
  _lastLcd      = 0;
  _state        = State::PLAYING;
  char buf[17];
  snprintf(buf, sizeof(buf), "Round %d / %d", r + 1, ROUND_COUNT);
  printLine(0, buf);
  printLine(1, "Tune the pot!");
  delay(600);
}

int FrequencyLock::potToHz(int raw) {
  return map(raw, 0, 1023, FREQ_MIN, FREQ_MAX);
}

void FrequencyLock::printLine(uint8_t row, const char* text) {
  _lcd.setCursor(0, row);
  _lcd.print("                ");
  _lcd.setCursor(0, row);
  _lcd.print(text);
}

void FrequencyLock::successTone() {
  tone(BUZZER_PIN, 1200, 90); delay(120);
  tone(BUZZER_PIN, 1700, 90); delay(120);
  tone(BUZZER_PIN, 2200, 120); delay(150);
}

void FrequencyLock::failTone() {
  tone(BUZZER_PIN, 250, 300); delay(350);
  tone(BUZZER_PIN, 180, 450); delay(500);
}
