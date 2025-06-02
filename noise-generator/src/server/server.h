#include <vector>
#include <string>
#include <Arduino.h>

void serverSetup();
void serverLoop();
void updateIsRecording(bool isRecording);
void updateActiveAudioId(int audioId);
void updateIsUltrasonic(bool isUltrasonic);
void updateAudioFiles(const std::vector<String>& audioFiles);