#include "MemorySequence.h"
#include "Config.h"

MemorySequence::MemorySequence(LiquidCrystal_I2C& lcd, TM1638plus& tm)
  : _lcd(lcd), _tm(tm),
    _state(State::SHOWING_ON),
    _seqLen(0), _showIdx(0), _inputIdx(0),
    _stateMs(0), _showOnMs(700), _showOffMs(350),
    _pauseStart(0), _prevButtons(0), _lastBtnRead(0)
{
  memset(_sequence, 0, sizeof(_sequence));
}

// ---------------------------------------------------------------------------

void MemorySequence::begin() {
  randomSeed(analogRead(A1));
  _tm.reset();
  startRound(0);
}

// ---------------------------------------------------------------------------

bool MemorySequence::update() {
  unsigned long now = millis();

  switch (_state) {

    // ---- Show one number of the sequence on 7-seg ----
    case State::SHOWING_ON:
      if (now - _stateMs >= _showOnMs) {
        _tm.displayText("        ");   // blank between steps
        _stateMs = now;
        _state = State::SHOWING_OFF;
      }
      break;

    // ---- Blank gap between numbers ----
    case State::SHOWING_OFF:
      if (now - _stateMs >= _showOffMs) {
        _showIdx++;
        if (_showIdx >= _seqLen) {
          // Whole sequence shown — player's turn
          lcdStatus("Memory Seq", "-> Your turn!");
          _tm.displayText("GO!     ");
          _inputIdx = 0;
          _state = State::PLAYER_TURN;
        } else {
          showStep(_showIdx);
          _stateMs = now;
          _state = State::SHOWING_ON;
        }
      }
      break;

    // ---- Wait for player input ----
    case State::PLAYER_TURN: {
      uint8_t pressed = readNewPress();
      if (!pressed) break;

      // Which button index (0-7)?
      uint8_t btn = 0;
      for (uint8_t i = 0; i < 8; i++) {
        if (pressed & (1 << i)) { btn = i; break; }
      }

      // Show pressed number as feedback on 7-seg
      char fb[9];
      snprintf(fb, sizeof(fb), "  [%d]   ", btn + 1);
      _tm.displayText(fb);
      tone(BUZZER_PIN, 400 + btn * 80, 80);
      delay(120);

      if (btn == _sequence[_inputIdx]) {
        _inputIdx++;

        char buf[17];
        snprintf(buf, sizeof(buf), "%d / %d correct", _inputIdx, _seqLen);
        lcdStatus("Memory Seq", buf);

        if (_inputIdx >= _seqLen) {
          successTone();
          _tm.displayText("NICE!   ");
          _pauseStart = millis();
          _state = State::ROUND_WIN;
        } else {
          // Show next expected index hint on 7-seg
          snprintf(fb, sizeof(fb), "%d/%d     ", _inputIdx + 1, _seqLen);
          _tm.displayText(fb);
        }
      } else {
        // Wrong — show what was expected
        char buf[17];
        snprintf(buf, sizeof(buf), "Wanted btn %d", _sequence[_inputIdx] + 1);
        lcdStatus("WRONG!", buf);
        char seg[9];
        snprintf(seg, sizeof(seg), "ERR  %d  ", _sequence[_inputIdx] + 1);
        _tm.displayText(seg);
        failTone();
        _state = State::FAILED;
      }
      break;
    }

    // ---- Brief pause then next round ----
    case State::ROUND_WIN:
      if (now - _pauseStart >= 1200) {
        uint8_t nextRound = _seqLen - START_LEN + 1;
        if (nextRound >= ROUNDS) return true;
        startRound(nextRound);
      }
      break;

    case State::FAILED:
      break;
  }

  return false;
}

// ---------------------------------------------------------------------------

void MemorySequence::onGameOver() {
  _tm.reset();
  lcdStatus("Comms OFFLINE", "");
}

// ---------------------------------------------------------------------------

void MemorySequence::startRound(uint8_t round) {
  _seqLen   = START_LEN + round;
  _showIdx  = 0;
  _inputIdx = 0;

  _showOnMs  = max(200UL, 700UL - round * 90UL);
  _showOffMs = max(100UL, 350UL - round * 45UL);

  for (uint8_t i = 0; i < _seqLen; i++) {
    _sequence[i] = random(0, 8);
  }

  char buf[17];
  snprintf(buf, sizeof(buf), "Round %d / %d", round + 1, ROUNDS);
  lcdStatus("Memory Seq", buf);
  char seg[9];
  snprintf(seg, sizeof(seg), "RND  %d  ", round + 1);
  _tm.displayText(seg);
  delay(800);

  lcdStatus("Memory Seq", "Watch & remember");
  _tm.displayText("WATCH   ");
  delay(600);

  showStep(0);
  _stateMs = millis();
  _state   = State::SHOWING_ON;
}

void MemorySequence::showStep(uint8_t idx) {
  // Show button number (1-based) centered on 7-segment
  char seg[9];
  snprintf(seg, sizeof(seg), "  -%d-   ", _sequence[idx] + 1);
  _tm.displayText(seg);
}

// ---------------------------------------------------------------------------

void MemorySequence::lcdStatus(const char* line0, const char* line1) {
  _lcd.setCursor(0, 0);
  _lcd.print("                ");
  _lcd.setCursor(0, 0);
  _lcd.print(line0);
  _lcd.setCursor(0, 1);
  _lcd.print("                ");
  _lcd.setCursor(0, 1);
  _lcd.print(line1);
}

uint8_t MemorySequence::readNewPress() {
  unsigned long now = millis();
  if (now - _lastBtnRead < 50) return 0;
  _lastBtnRead = now;
  uint8_t btns     = _tm.readButtons();
  uint8_t newPress = btns & ~_prevButtons;
  _prevButtons     = btns;
  return newPress;
}

void MemorySequence::successTone() {
  tone(BUZZER_PIN, 1000, 80); delay(100);
  tone(BUZZER_PIN, 1400, 80); delay(100);
  tone(BUZZER_PIN, 1800, 120); delay(150);
}

void MemorySequence::failTone() {
  tone(BUZZER_PIN, 300, 400); delay(450);
  tone(BUZZER_PIN, 200, 500); delay(550);
}
