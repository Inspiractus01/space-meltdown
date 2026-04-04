// =============================================================================
// SpaceMeltdown.ino — Entry point
// Space Station Meltdown — Embedded Programming Project
// Author: Michal Mucha <michalmucha@proton.me>
// =============================================================================

#include "GameManager.h"

GameManager gm;

void setup() {
  gm.begin();
}

void loop() {
  gm.update();
}
