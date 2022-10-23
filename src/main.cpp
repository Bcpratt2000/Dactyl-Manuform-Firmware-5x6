#include <Arduino.h>
#include <PluggableUSBHID.h>
#include <USBHID_Types.h>
#include <USBKeyboard.h>

USBKeyboard keyboard;

#define LED 25
#define SLEW_US 10 // time is microseconds to wait between setting the pin voltage to high and reading the output value

// Is this compiled for the left half or the right half
//  #define LEFT
#define RIGHT

//define the size of the keyboard
#define MATRIX_HEIGHT 6
#define MATRIX_WIDTH 6

// define history of keypresses
bool lastState[MATRIX_WIDTH][MATRIX_HEIGHT];
bool currentState[MATRIX_WIDTH][MATRIX_HEIGHT];

//define the pins used for the key matrix
short outputPins[MATRIX_HEIGHT] = {0, 1, 2, 3, 4, 5};
short inputPins[MATRIX_WIDTH] = {6, 7, 8, 9, 10, 11};

// Define Keymaps
#ifdef LEFT
short Keymap[MATRIX_WIDTH][MATRIX_HEIGHT] = {
    {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
#endif
#ifdef RIGHT
short Keymap[MATRIX_WIDTH][MATRIX_HEIGHT] = {
    {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
#endif

void setup() {
  pinMode(LED, OUTPUT);

  // set output and input pinmodes
  for (int i = 0; i < MATRIX_HEIGHT; i++) {
    pinMode(outputPins[i], OUTPUT);
  }
  for (int i = 0; i < MATRIX_WIDTH; i++) {
    pinMode(inputPins[i], INPUT_PULLDOWN);
  }

  // delay(5000);
  // keyboard.printf("Hello World\r\n");
}

// Entry Point
void loop() {
  // loop through button matrix and fill currentState[][].
  for (int i = 0; i < MATRIX_WIDTH; i++) {
    digitalWrite(outputPins[i], HIGH);
    delayMicroseconds(SLEW_US);
    for (int j = 0; j < MATRIX_HEIGHT; j++) {
      currentState[i][j] = digitalRead(outputPins[j]);
    }
    digitalWrite(outputPins[i], LOW);
  }

  // loop through states and update the button press state.
  for (int i = 0; i < MATRIX_WIDTH; i++) {
    for (int j = 0; j < MATRIX_HEIGHT; j++) {
      if (currentState[i][j] != lastState[i][j]) {
        if (currentState[i][j]) {
          // press button
        } else {
          // release button
        }
        lastState[i][j] = currentState[i][j];
      }
    }
  }
}
