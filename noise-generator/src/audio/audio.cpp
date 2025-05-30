#include "audio.h"
#include "config/config.h"
#include "events/events.h"
#include "display/display.h"
#include "wav/wav_utils.h"
#include <SD.h>
#include "sd/sd_card.h"
#include <driver/i2s.h>
#include <Arduino.h>
#include <stdio.h>
#include <freertos/FreeRTOS.h>

static TaskHandle_t playbackTaskHandle = nullptr;
static TaskHandle_t recordingTaskHandle = nullptr;
bool isPlaying = false;
bool isRecording = false;
bool isUltrasonic = false;
static const i2s_port_t i2s_num = I2S_NUM_0; // i2s port number
unsigned const char *TheData;
uint32_t DataIdx = 0;
struct WavHeader_Struct WavHeader;
static String currentFilenameStr;

void startUltrasound()
{
    // Встановлення заповнення 50% для ШІМ
    isUltrasonic = true;
    ledcWrite(ULTRASOUND_CHANNEL, 128); // 128 із 255 (50%)
}

void stopUltrasound()
{
    // Зупинка ШІМ
    isUltrasonic = false;
    ledcWrite(ULTRASOUND_CHANNEL, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// Ініціалізація I2S
void setupAudio()
{
    i2s_driver_install(i2s_num, &i2s_speaker_config, 0, NULL);
    i2s_set_pin(i2s_num, &i2s_speaker_pins);
    ledcSetup(ULTRASOUND_CHANNEL, ULTRASOUND_FREQUENCY, ULTRASOUND_RESOLUTION);
    ledcAttachPin(ULTRASOUND_PIN, ULTRASOUND_CHANNEL);
    stopUltrasound();
    analogReadResolution(12); // або 16
    analogSetAttenuation(ADC_11db);
}

void stopRecording()
{
    drawMessage("Saving...");
    sleep(1);
    updateListFiles();
    drawCurrentFile(); 

    Serial.println("Stopping recording...");
    isRecording = false;

    if (recordingTaskHandle != nullptr)
    {
        vTaskSuspend(recordingTaskHandle);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
void recordTask(void *param)
{
    // const char *filename = currentFilenameStr.c_str();
    const char *filename = (const char *)param;
    const int sampleRate = 16000;
    const int bitsPerSample = 12; // 12 біт для ADC
    const int adcPin = 36;        // VP pin

    analogReadResolution(bitsPerSample);
    analogSetAttenuation(ADC_11db);
    Serial.println("record task: " + String(filename));

    File file = SD.open(filename, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        isRecording = false;
        vTaskDelete(nullptr);
        return;
    }

    // Пишемо тимчасовий WAV-заголовок
    WriteWavHeader(file, 0);

    uint32_t bytesWritten = 0;
    uint32_t startMillis = millis();

    while (isRecording && (millis() - startMillis < 15000)) // 15 секунд або прапорець
    {
        uint16_t sample = analogRead(adcPin);                 // 0–4095
        int16_t signedSample = ((int32_t)sample - 2048) << 4; // перетворення в signed 16-bit

        file.write((uint8_t *)&signedSample, sizeof(int16_t));
        bytesWritten += sizeof(int16_t);

        delayMicroseconds(1000000 / sampleRate);
    }

    // Перезапис заголовка з актуальним розміром
    file.seek(0);
    WriteWavHeader(file, bytesWritten);
    file.close();
    free((void*)filename);
    Serial.println("Recording stopped.");
    stopRecording();
}

void startRecording(String filename)
{
    drawMessage("Recording...");
    Serial.println("FIle name start recording: " + filename);
    isRecording = true;
    currentFilenameStr = filename;
    if (recordingTaskHandle == nullptr)
    {
        char *filenameCopy = strdup(filename.c_str());
        xTaskCreatePinnedToCore(recordTask, "RecordTask", 4096, filenameCopy, 1, &recordingTaskHandle, 0);
    }
    else
    {
        isRecording = true;
        vTaskResume(recordingTaskHandle);
    }
}

void playbackTask(void *param)
{
    // const char *filename = (const char *)param;
    const char *filename = currentFilenameStr.c_str();

    while (true)
    {
        if (!isPlaying)
        {
            vTaskSuspend(nullptr); // Суспендить сама себе
        }

        File file = SD.open(filename, FILE_READ);
        if (!file)
        {
            Serial.println("Failed to open file for playback.");
            continue;
        }

        // Зчитування заголовка WAV
        file.read((uint8_t *)&WavHeader, sizeof(WavHeader_Struct));
        if (!ValidWavData(&WavHeader))
        {
            Serial.println("Invalid WAV file.");
            file.close();
            continue;
        }

        DumpWAVHeader(&WavHeader);
        i2s_set_sample_rates(i2s_num, WavHeader.SampleRate);

        size_t bytesRead;
        size_t bytesWritten;
        const int bufferSize = 512;
        uint8_t buffer[bufferSize];

        while (file.available() && isPlaying)
        {
            bytesRead = file.read(buffer, bufferSize);

            if (WavHeader.NumChannels == 1)
            {
                uint8_t stereoBuffer[bufferSize * 2];
                for (int i = 0, j = 0; i < bytesRead; i += 2, j += 4)
                {
                    stereoBuffer[j] = buffer[i];
                    stereoBuffer[j + 1] = buffer[i + 1];
                    stereoBuffer[j + 2] = buffer[i];
                    stereoBuffer[j + 3] = buffer[i + 1];
                }
                i2s_write(i2s_num, stereoBuffer, bytesRead * 2, &bytesWritten, portMAX_DELAY);
            }
            else
            {
                i2s_write(i2s_num, buffer, bytesRead, &bytesWritten, portMAX_DELAY);
            }
        }

        file.close();
        Serial.println("Playback finished or stopped.");
    }
}

void startPlayback(String filename)
{
    digitalWrite(LED_BLUE, HIGH);
    currentFilenameStr = filename;
    startUltrasound();

    if (playbackTaskHandle == nullptr)
    {
        const char *filenameC = currentFilenameStr.c_str();
        isPlaying = true;
        xTaskCreatePinnedToCore(playbackTask, "PlaybackTask", 4096, (void *)filenameC, 1, &playbackTaskHandle, 0);
    }
    else
    {
        isPlaying = true;
        vTaskResume(playbackTaskHandle);
    }
}

void stopPlayback()
{
    Serial.println("Stopping playback...");
    isPlaying = false;
    drawMessage("Stopped...");
    digitalWrite(LED_BLUE, LOW);
    sleep(1);
    drawCurrentFile();
    stopUltrasound();

    if (playbackTaskHandle != nullptr)
    {
        vTaskSuspend(playbackTaskHandle);
    }
}