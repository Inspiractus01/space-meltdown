// Space Station Meltdown — Embedded escape room on Nucleo F303RE

#include "GameManager.h"

GameManager gm;

void setup() {
  gm.begin();
}

void loop() {
  gm.update();
}
