#include <Arduino.h>
// #include "PluggableUSBHID.h"
// #include "USBKeyboard.h"
#include "Adafruit_TinyUSB.h"

#define SIDE 0 //0 is left, 1 is right

int lastState[4][8];
int currentState[4][8];

// int Keymap[4][8] ={0, 0, 0, 0, 0, 0, 0, 0},
//                   {0, 0, 0, 0, 0, 0, 0, 0},
//                   {0, 0, 0, 0, 0, 0, 0, 0},
//                   {0, 0, 0, 0, 0, 0, 0, 0};
void setup() {
  // put your setup code here, to run once:
  TinyUSB_Device_Init(0);
}

void loop() {
  // put your main code here, to run repeatedly:
}