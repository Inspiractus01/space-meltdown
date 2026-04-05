#include <TM1638plus.h>

#define STB_PIN  7
#define CLK_PIN  8
#define DIO_PIN  9

TM1638plus tm(STB_PIN, CLK_PIN, DIO_PIN, false);

void setup() {
  tm.displayBegin();
  tm.setLEDs(0xFF);
  tm.displayText("HELLO   ");
}

void loop() {
  uint8_t btn = tm.readButtons();
  tm.setLEDs(btn);
}
