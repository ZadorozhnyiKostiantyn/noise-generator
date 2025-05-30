#include <Arduino.h>
#include "config/config.h"
#include "display/display.h"
#include "sd/sd_card.h"
#include "audio/audio.h"
#include "server/server.h"
#include <vector>
#include <string>

int currentFileIndex = 0;

//
// ─── UTILS ────────────────────────────────────────────────────────────────────
//
using EventCallback = void (*)();

void drawCurrentFile()
{
  std::vector<String> files = getListFiles();
  if (files.empty())
    return;
  if (currentFileIndex >= files.size())
    return; // Перевірка, чи currentFileIndex виходить за межі списку
  drawMessage(String(currentFileIndex + 1) + ": " + files[currentFileIndex]);
}

void toggleRecording()
{
  if (isRecording)
  {
    stopRecording();
  }
  else
  {
    String newFileName = "/recording_" + String(millis()) + ".wav";
    startRecording(newFileName);
  }
}

void togglePlayback()
{
  Serial.println("Toggling playback...");
  Serial.println("isPlaying: " + String(isPlaying ? "true" : "false"));
  if (isPlaying)
  {
    stopPlayback();
  }
  else
  {
    std::vector<String> files = getListFiles();
    if (currentFileIndex >= files.size())
    {
      drawMessage("Invalid file index");
      return;
    }
    String currentFile = "/" + files[currentFileIndex];
    String currentFileDisplay = "> " + files[currentFileIndex];
    drawMessage(currentFileDisplay);
    startPlayback(currentFile);
  }
}

//
// ─── EVENTS ───────────────────────────────────────────────────────────────
//

void recordingEvent(String filename)
{
  toggleRecording();
  digitalWrite(LED_RED, isPlaying ? HIGH : LOW);
}

void audioEvent(int id)
{
  currentFileIndex = id;
  togglePlayback();
  digitalWrite(LED_BLUE, isPlaying ? HIGH : LOW);
}

void ultrasonicEvent(bool toggle)
{
  isUltrasonic = toggle;
}

//
// ─── BUTTON HANDLER ───────────────────────────────────────────────────────────────
//

void handleNavigateToNextFile()
{
  std::vector<String> files = getListFiles();
  currentFileIndex = (currentFileIndex + 1) % files.size();
  drawCurrentFile();
}

void handleNavigateToPreviousFile()
{
  std::vector<String> files = getListFiles();
  currentFileIndex = (currentFileIndex + files.size() - 1) % files.size();
  drawCurrentFile();
}

void handleTogglePlayback()
{
  togglePlayback();
  updateActiveAudioId(isPlaying ? -1 : currentFileIndex);
}

void handleRecordingToggle()
{
  toggleRecording();

  if(!isRecording) {
    updateAudioFiles(getListFiles());
  }

  updateIsRecording(isRecording);
}
//
// ─── BUTTON CONFIG ───────────────────────────────────────────────────────────────
//

struct ButtonConfig
{
  uint8_t buttonPin;
  uint8_t mask;
  const char *message;
  uint8_t ledPin;
  EventCallback onClick;
};

ButtonConfig BUTTONS_CONFIG[4] = {
    {BUTTON_YELLOW, YELLOW_MASK, "", LED_YELLOW, handleNavigateToPreviousFile},
    {BUTTON_GREEN, GREEN_MASK, "", LED_GREEN, handleNavigateToNextFile},
    {BUTTON_BLUE, BLUE_MASK, "", LED_BLUE, handleTogglePlayback},
    {BUTTON_RED, RED_MASK, "", LED_RED, handleRecordingToggle}};
constexpr size_t BUTTON_COUNT = sizeof(BUTTONS_CONFIG) / sizeof(BUTTONS_CONFIG[0]);

//
// ─── EVENTS ──────────────────────────────────────────────────────────────────────
//

constexpr size_t MAX_EVENTS = BUTTON_COUNT;

void *event_list[MAX_EVENTS] = {nullptr}; // Список подій
uint8_t event_masks[MAX_EVENTS] = {0};    // Відповідні маски кнопок

void register_event(EventCallback event, uint8_t button_mask)
{
  for (size_t i = 0; i < MAX_EVENTS; ++i)
  {
    if (event_list[i] == nullptr)
    {
      event_list[i] = reinterpret_cast<void *>(event);
      event_masks[i] = button_mask;
      break;
    }
  }
}

void run_events(uint8_t button_mask)
{
  for (size_t i = 0; i < MAX_EVENTS; ++i)
  {
    if ((event_list[i] != nullptr) && (event_masks[i] & button_mask))
    {
      auto callback = reinterpret_cast<EventCallback>(event_list[i]);
      callback();
    }
  }
}

void turnOffAllLeds()
{
  for (const auto &config : BUTTONS_CONFIG)
  {
    digitalWrite(config.ledPin, LOW);
  }
  if(isPlaying) {
    stopPlayback();
  }

  if(isRecording) {
    stopRecording();
  }
}

//
// ─── BUTTON TASK ─────────────────────────────────────────────────────────────────
//

void taskButtonEvent(void *pvParameters)
{
  for (const auto &config : BUTTONS_CONFIG)
  {
    pinMode(config.buttonPin, INPUT_PULLUP);
    pinMode(config.ledPin, OUTPUT);
    digitalWrite(config.ledPin, LOW);
  }

  uint8_t lastButtonStateMASK = BUTTON_MASK;
  uint8_t buttonStateMASK = BUTTON_MASK;

  for (;;)
  {
    buttonStateMASK = 0;
    for (size_t i = 0; i < BUTTON_COUNT; ++i)
    {
      buttonStateMASK |= (digitalRead(BUTTONS_CONFIG[i].buttonPin) << i);
    }

    if (lastButtonStateMASK != buttonStateMASK)
    {
      for (size_t i = 0; i < BUTTON_COUNT; ++i)
      {
        const auto &config = BUTTONS_CONFIG[i];
        if (
            (buttonStateMASK & config.mask) != (lastButtonStateMASK & config.mask) &&
            digitalRead(config.buttonPin) == BUTTON_PRESSED)
        {
          if (config.message != "")
          {
            drawMessage(config.message);
          }
          turnOffAllLeds();
          digitalWrite(config.ledPin, HIGH);
          if (config.onClick)
            config.onClick(); // Викликається тут, але також можна з run_events()
        }
      }

      lastButtonStateMASK = buttonStateMASK;
    }

    vTaskDelay(10);
  }
}

//
// ─── INIT EVENTS ────────────────────────────────────────────────────────────────
//

void setupEvents()
{
  for (const auto &config : BUTTONS_CONFIG)
  {
    register_event(config.onClick, config.mask);
  }

  drawCurrentFile();
}
