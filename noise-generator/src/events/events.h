#pragma once
#include <cstddef>
#include <cstdint>

void setupEvents();
void run_event(uint8_t button_mask);
void register_event(void (*event)(), uint8_t button_mask);
void turnOffAllLeds();
void taskButtonEvent(void *pvParameters);

void recordingEvent(String filename);
void audioEvent(int id);
void ultrasonicEvent(bool value);
void drawCurrentFile();

struct ButtonConfig {
  uint8_t buttonPin;
  uint8_t mask;
  const char* message;
  uint8_t ledPin;
  void (*onClick)();
};

extern int currentFileIndex;

