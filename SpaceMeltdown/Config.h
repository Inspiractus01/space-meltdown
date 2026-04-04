// =============================================================================
// Config.h — Pin definitions and global constants
// Space Station Meltdown
// Author: Michal Mucha <michalmucha@proton.me>
// =============================================================================

#pragma once

// --- LCD (I2C) ---
#define LCD_I2C_ADDR  0x27
#define LCD_COLS      16
#define LCD_ROWS       2

// --- TM1638 (LED&KEY) ---
#define TM_STB_PIN    7
#define TM_CLK_PIN    8
#define TM_DIO_PIN    9

// --- Potentiometer ---
#define POT_PIN       A0

// --- Passive Buzzer ---
#define BUZZER_PIN    6

// --- Discrete LEDs ---
#define LED_RED_PIN   3
#define LED_GREEN_PIN 4
#define LED_YELLOW_PIN 5

// --- RGB LED (common cathode) ---
#define RGB_R_PIN    10
#define RGB_G_PIN    11
#define RGB_B_PIN    12

// --- Push buttons (external) ---
#define BTN_PIN       2   // spare external button

// --- USER button on Nucleo ---
#define USER_BTN_PIN  PC13

// --- Game settings ---
#define TOTAL_CHALLENGES  4
#define GAME_TIME_SECONDS 480   // 8 minutes
