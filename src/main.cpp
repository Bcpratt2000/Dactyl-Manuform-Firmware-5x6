#include <Arduino.h>
// #include "PluggableUSBHID.h"
// #include "USBKeyboard.h"
#include <PluggableUSBHID.h>
#include <USBHID_Types.h>
#include <USBKeyboard.h>

USBKeyboard keyboard;

#define LED 25

// Is this compiled for the left half or the right half
//  #define LEFT
#define RIGHT

// define history of keypresses
int lastState[6][6];
int currentState[6][6];

// Define Keymaps
#ifdef LEFT
int Keymap[6][6] = {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
#endif
#ifdef RIGHT
int Keymap[6][6] = {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
#endif


void setup() {}

// Entry Point
void loop() {
  pinMode(LED, OUTPUT);

  delay(5000);
  keyboard.printf("Hello World\r\n");

  while (1) {
    digitalWrite(LED, true);
    delay(1000);
    digitalWrite(LED, false);
    delay(1000);
  }
}
