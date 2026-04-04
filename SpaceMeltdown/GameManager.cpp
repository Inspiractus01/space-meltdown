// =============================================================================
// GameManager.cpp
// Space Station Meltdown
// Author: Michal Mucha <michalmucha@proton.me>
// =============================================================================

#include "GameManager.h"

// Include all challenge headers here when they are ready.
// #include "FrequencyLock.h"
// #include "MemorySequence.h"
// #include "ReactorCode.h"
// #include "StabilizeCore.h"

#include <LiquidCrystal_I2C.h>

// Shared LCD instance (all challenges can access it via extern if needed).
LiquidCrystal_I2C lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);

// USER button on Nucleo — used to start the game from INTRO screen.
volatile bool startPressed = false;
void onStartBtn() { startPressed = true; }

// ---------------------------------------------------------------------------

GameManager::GameManager()
  : _state(GameState::INTRO),
    _currentIndex(0),
    _gameStartMs(0),
    _transitionStartMs(0)
{
  // Register challenges in order.
  // Replace nullptr with actual objects once each challenge is implemented.
  _challenges[0] = nullptr; // new FrequencyLock(lcd)
  _challenges[1] = nullptr; // new MemorySequence()
  _challenges[2] = nullptr; // new ReactorCode(lcd)
  _challenges[3] = nullptr; // new StabilizeCore(lcd)
}

// ---------------------------------------------------------------------------

void GameManager::begin() {
  lcd.init();
  lcd.backlight();

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
        startChallenge(0);
      }
      break;

    case GameState::RUNNING: {
      // Check timer
      if (timeRemainingSeconds() <= 0) {
        _state = GameState::GAME_OVER;
        playGameOver();
        break;
      }

      showHUD();

      // Tick the active challenge
      Challenge* c = _challenges[_currentIndex];
      if (c && c->update()) {
        // Challenge solved
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
      // Wait 2 seconds, then start next challenge
      if (millis() - _transitionStartMs >= 2000) {
        startChallenge(_currentIndex);
      }
      break;

    case GameState::WIN:
    case GameState::GAME_OVER:
      // Nothing — final screen stays until reset
      break;
  }
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

int GameManager::timeRemainingSeconds() const {
  unsigned long elapsed = (millis() - _gameStartMs) / 1000UL;
  int remaining = GAME_TIME_SECONDS - (int)elapsed;
  return remaining < 0 ? 0 : remaining;
}

void GameManager::startChallenge(int index) {
  _currentIndex = index;
  _state = GameState::RUNNING;
  if (_challenges[index]) {
    _challenges[index]->begin();
  }
}

void GameManager::showHUD() const {
  // Line 0: challenge name (updated by challenge itself)
  // Line 1: time remaining
  int t = timeRemainingSeconds();
  int mins = t / 60;
  int secs = t % 60;

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  if (mins < 10) lcd.print('0');
  lcd.print(mins);
  lcd.print(':');
  if (secs < 10) lcd.print('0');
  lcd.print(secs);
  lcd.print("  Sys:");
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
    tone(BUZZER_PIN, 1500, 150);
    delay(200);
    tone(BUZZER_PIN, 2000, 150);
    delay(200);
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
