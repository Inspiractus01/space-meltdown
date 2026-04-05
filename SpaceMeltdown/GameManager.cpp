#include "GameManager.h"
#include "FrequencyLock.h"
#include "MemorySequence.h"
#include "ReactorCode.h"
// #include "StabilizeCore.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
TM1638plus        tm(TM_STB_PIN, TM_CLK_PIN, TM_DIO_PIN, false);

volatile bool startPressed = false;
void onStartBtn() { startPressed = true; }

// ---------------------------------------------------------------------------

GameManager::GameManager()
  : _state(GameState::INTRO),
    _currentIndex(0),
    _gameStartMs(0),
    _transitionStartMs(0)
{
  _challenges[0] = new FrequencyLock(lcd);
  _challenges[1] = new MemorySequence(lcd, tm);
  _challenges[2] = new ReactorCode(lcd, tm);
  _challenges[3] = nullptr; // new StabilizeCore(lcd)
}

// ---------------------------------------------------------------------------

void GameManager::begin() {
  delay(1000);
  Wire.setSDA(D14);
  Wire.setSCL(D15);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  tm.displayBegin();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(USER_BTN_PIN), onStartBtn, FALLING);

  showIntro();
}

// ---------------------------------------------------------------------------

void GameManager::update() {
  switch (_state) {

    case GameState::INTRO:
      if (startPressed) {
        startPressed = false;
        _gameStartMs = millis();
        startChallenge(2); // TODO: change back to 0 before final submission
      }
      break;

    case GameState::RUNNING: {
      if (timeRemainingSeconds() <= 0) {
        _state = GameState::GAME_OVER;
        playGameOver();
        break;
      }

      // Challenge owns the full display during RUNNING state
      Challenge* c = _challenges[_currentIndex];
      if (c && c->update()) {
        _transitionStartMs = millis();
        showTransition(_currentIndex);
        _currentIndex++;
        if (_currentIndex >= TOTAL_CHALLENGES) {
          _state = GameState::WIN;
          playWin();
        } else {
          _state = GameState::TRANSITION;
        }
      }
      break;
    }

    case GameState::TRANSITION:
      if (millis() - _transitionStartMs >= 2000) {
        startChallenge(_currentIndex);
      }
      break;

    case GameState::WIN:
    case GameState::GAME_OVER:
      break;
  }
}

// ---------------------------------------------------------------------------

int GameManager::timeRemainingSeconds() const {
  unsigned long elapsed = (millis() - _gameStartMs) / 1000UL;
  int remaining = GAME_TIME_SECONDS - (int)elapsed;
  return remaining < 0 ? 0 : remaining;
}

void GameManager::startChallenge(int index) {
  _currentIndex = index;
  _state = GameState::RUNNING;
  if (_challenges[index]) _challenges[index]->begin();
}

void GameManager::showHUD() const {
  int t    = timeRemainingSeconds();
  int mins = t / 60;
  int secs = t % 60;
  lcd.setCursor(0, 1);
  lcd.print("T:");
  if (mins < 10) lcd.print('0');
  lcd.print(mins);
  lcd.print(':');
  if (secs < 10) lcd.print('0');
  lcd.print(secs);
  lcd.print(" Sys:");
  lcd.print(_currentIndex + 1);
  lcd.print('/');
  lcd.print(TOTAL_CHALLENGES);
}

void GameManager::showIntro() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SPACE MELTDOWN");
  lcd.setCursor(0, 1);
  lcd.print("Press USER btn");
}

void GameManager::showTransition(int completedIndex) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System ");
  lcd.print(completedIndex + 1);
  lcd.print(" FIXED!");
  lcd.setCursor(0, 1);
  lcd.print("Next system...");
  tone(BUZZER_PIN, 1000, 300);
}

void GameManager::playWin() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("STATION SAVED!");
  lcd.setCursor(0, 1);
  lcd.print("You're a hero!");
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_PIN, 1500, 150); delay(200);
    tone(BUZZER_PIN, 2000, 150); delay(200);
  }
}

void GameManager::playGameOver() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("STATION LOST...");
  lcd.setCursor(0, 1);
  lcd.print("Game Over");
  tone(BUZZER_PIN, 200, 1000);
}
