#ifndef DEFINE_H
#define DEFINE_H

#ifndef ARDUINO
#define ARDUINO 10800
#endif

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
// #include <SD.h>
#include "portSettings.h"

extern Adafruit_PCD8544 display;
// File root;

extern const int directionThreshold;
extern const int snakeInitialSpeed;
extern int snakeSpeed;
extern const unsigned int snakeXPositions;
extern const unsigned int snakeYPositions;

extern const uint8_t displayWidth;
extern const uint8_t displayHeight;

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#endif

#endif
