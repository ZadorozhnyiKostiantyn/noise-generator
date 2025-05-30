#include "sd/sd_card.h"
#include "audio/audio.h"
#include "display/display.h"
#include "config/config.h"
#include "events/events.h"
#include "server/server.h"


void setup() {
  Serial.begin(115200);
  setupSDCard();
  setupAudio();
  setupDisplay();
  setupEvents();
  serverSetup();

  xTaskCreatePinnedToCore(
    taskButtonEvent,
    "taskButtonEvent",
    10000,
    NULL,
    1,
    NULL,
    0);
}

void loop() {
  vTaskDelay(1000);
  serverLoop();
}

