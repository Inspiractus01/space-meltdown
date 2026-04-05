#include "ReactorCode.h"
#include "Config.h"

const uint8_t      ReactorCode::CODES_PER_ROUND[ROUNDS] = { 2, 3, 4, 5 };
const unsigned long ReactorCode::SHOW_TIME[ROUNDS]       = { 4000, 3000, 2000, 1500 };

// ---------------------------------------------------------------------------

ReactorCode::ReactorCode(LiquidCrystal_I2C& lcd, TM1638plus& tm)
  : _lcd(lcd), _tm(tm),
    _state(State::SHOWING),
    _round(0), _codesDone(0), _answer(0), _lives(2),
    _showStart(0), _showDuration(4000), _pauseStart(0),
    _prevButtons(0), _lastBtnRead(0)
{}

// ---------------------------------------------------------------------------

void ReactorCode::begin() {
  randomSeed(analogRead(A1));
  _round     = 0;
  _codesDone = 0;
  _lives     = 2;
  lcdStatus("Reactor Code", "System 3...");
  _tm.displayText("REACTOR ");
  delay(1000);
  nextCode();
}

// ---------------------------------------------------------------------------

bool ReactorCode::update() {
  unsigned long now = millis();

  switch (_state) {

    // ---- Show binary number for limited time ----
    case State::SHOWING:
      if (now - _showStart >= _showDuration) {
        _tm.displayText("        "); // blank — enter from memory
        char buf[17];
        snprintf(buf, sizeof(buf), "Enter (lives:%d)", _lives);
        lcdStatus("What decimal?", buf);
        _state = State::WAITING;
      }
      break;

    // ---- Player presses a button ----
    case State::WAITING: {
      uint8_t pressed = readNewPress();
      if (!pressed) break;

      uint8_t btn = 0;
      for (uint8_t i = 0; i < 8; i++) {
        if (pressed & (1 << i)) { btn = i; break; }
      }

      // Show what was pressed
      char seg[9];
      snprintf(seg, sizeof(seg), "  [%d]   ", btn + 1);
      _tm.displayText(seg);
      tone(BUZZER_PIN, 500 + btn * 60, 80);
      delay(100);

      if (btn == _answer) {
        successTone();
        _tm.displayText("GOOD!   ");
        lcdStatus("Correct!", "");
        _codesDone++;
        _pauseStart = millis();
        _state = State::CODE_WIN;
      } else {
        _lives--;
        if (_lives == 0) {
          char buf[17];
          snprintf(buf, sizeof(buf), "Answer was %d", _answer + 1);
          lcdStatus("WRONG! FAIL", buf);
          _tm.displayText("FAIL    ");
          failTone();
          _state = State::FAILED;
        } else {
          char buf[17];
          snprintf(buf, sizeof(buf), "Wrong! Lives:%d", _lives);
          lcdStatus("Try again", buf);
          failTone();
          delay(600);
          // Show binary again
          showBinary(_answer);
          _showStart = millis();
          _state = State::SHOWING;
        }
      }
      break;
    }

    // ---- Pause between codes ----
    case State::CODE_WIN:
      if (now - _pauseStart >= 800) {
        if (_codesDone >= CODES_PER_ROUND[_round]) {
          // Round complete
          _round++;
          if (_round >= ROUNDS) return true;
          _codesDone = 0;
          _lives     = 2;
          char buf[17];
          snprintf(buf, sizeof(buf), "Round %d / %d", _round + 1, ROUNDS);
          lcdStatus("Round cleared!", buf);
          _tm.displayText("        ");
          delay(1000);
        }
        nextCode();
      }
      break;

    case State::ROUND_WIN:
      if (now - _pauseStart >= 1000) return true;
      break;

    case State::FAILED:
      break;
  }

  return false;
}

// ---------------------------------------------------------------------------

void ReactorCode::onGameOver() {
  _tm.reset();
  lcdStatus("Reactor BREACH", "");
}

// ---------------------------------------------------------------------------

void ReactorCode::nextCode() {
  _answer       = random(0, 8);       // value 1-8, stored as 0-7
  _showDuration = SHOW_TIME[_round];

  char buf[17];
  snprintf(buf, sizeof(buf), "Code %d/%d  R%d",
           _codesDone + 1, CODES_PER_ROUND[_round], _round + 1);
  lcdStatus("Reactor Code", buf);

  showBinary(_answer);
  _showStart = millis();
  _state     = State::SHOWING;
}

void ReactorCode::showBinary(uint8_t val) {
  // val is 0-7, display as 3-bit binary: e.g. val=5 → "b 1 0 1  "
  uint8_t v = val + 1; // 1-8, show as 3-4 bit
  char seg[9];
  seg[0] = 'b';
  seg[1] = ' ';
  seg[2] = (v & 0x08) ? '1' : '0';
  seg[3] = (v & 0x04) ? '1' : '0';
  seg[4] = (v & 0x02) ? '1' : '0';
  seg[5] = (v & 0x01) ? '1' : '0';
  seg[6] = ' ';
  seg[7] = ' ';
  seg[8] = '\0';
  _tm.displayText(seg);
}

void ReactorCode::lcdStatus(const char* l0, const char* l1) {
  _lcd.setCursor(0, 0);
  _lcd.print("                ");
  _lcd.setCursor(0, 0);
  _lcd.print(l0);
  _lcd.setCursor(0, 1);
  _lcd.print("                ");
  _lcd.setCursor(0, 1);
  _lcd.print(l1);
}

uint8_t ReactorCode::readNewPress() {
  unsigned long now = millis();
  if (now - _lastBtnRead < 50) return 0;
  _lastBtnRead = now;
  uint8_t btns     = _tm.readButtons();
  uint8_t newPress = btns & ~_prevButtons;
  _prevButtons     = btns;
  return newPress;
}

void ReactorCode::successTone() {
  tone(BUZZER_PIN, 1000, 80); delay(100);
  tone(BUZZER_PIN, 1500, 80); delay(100);
  tone(BUZZER_PIN, 2000, 120); delay(150);
}

void ReactorCode::failTone() {
  tone(BUZZER_PIN, 300, 300); delay(350);
  tone(BUZZER_PIN, 200, 400); delay(450);
}
