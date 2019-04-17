#include "define.h"

Adafruit_PCD8544 display = Adafruit_PCD8544(pDisplayDC, pDisplaySCE, pDisplayRST);

const int directionThreshold = 250;
const int snakeInitialSpeed = 500;
int snakeSpeed;
const unsigned int snakeXPositions = 21;
const unsigned int snakeYPositions = 12;
const uint8_t displayWidth = 84;
const uint8_t displayHeight = 48;
