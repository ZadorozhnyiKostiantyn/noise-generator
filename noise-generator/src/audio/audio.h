#pragma once

#include <Arduino.h>

void setupAudio();
void startPlayback(String filename);
void stopPlayback();
void startRecording(String filename);
void stopRecording();
void startUltrasound();
void stopUltrasound();

extern bool isPlaying; 
extern bool isRecording; 
extern bool isUltrasonic; 


