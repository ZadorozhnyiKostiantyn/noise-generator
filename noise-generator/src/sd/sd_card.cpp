#include "config/config.h"
#include <SD.h>
#include <vector>
#include <Arduino.h>

std::vector<String> files;

std::vector<String> listFiles()
{
    std::vector<String> fileList;

    File root = SD.open("/");
    if (!root || !root.isDirectory())
    {
        Serial.println("Failed to open root directory or not a directory.");
        return fileList;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (!file.isDirectory())
        {
            fileList.push_back(String(file.name()));
        }
        file.close();
        file = root.openNextFile();
    }
    if (!file)
        file.close();
    root.close();
    return fileList;
}

void setupSDCard()
{
    pinMode(PIN_NUM_CS, OUTPUT);
    digitalWrite(PIN_NUM_CS, HIGH);

    if (!SD.begin(PIN_NUM_CS))
    {
        Serial.println("Failed to initialize SD card.");
        while (true)
            ;
    }
    Serial.println("SD card initialized.");
    files = listFiles();
}

std::vector<String> getListFiles()
{
    return files;
}

void updateListFiles()
{
    files = listFiles();
}
