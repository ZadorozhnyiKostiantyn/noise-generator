#pragma once

#include <vector>
#include <Arduino.h>


void setupSDCard();
void updateListFiles();
std::vector<String> getListFiles();
