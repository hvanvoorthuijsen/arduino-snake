#ifndef PORTSETTINGS_H
#define PORTSETTINGS_H

#include <Arduino.h>

extern const uint8_t pButtonA;
#define mButtonA B00000001 // button mask
extern const uint8_t pButtonB;
#define mButtonB B00000010 // button mask

extern const uint8_t pJoyStickX;
extern const uint8_t pJoyStickY;
extern const uint8_t pJoyStickButton;
#define mJoyStickButton B00000100 // button mask

extern byte buttonStates;

extern const uint8_t pDisplaySCE;
extern const uint8_t pDisplayRST;
extern const uint8_t pDisplayDC;
extern const uint8_t pDisplayDN;
extern const uint8_t pDisplaySCLK;
extern const uint8_t pDisplayLED;

extern const uint8_t pSDcs;

#endif
