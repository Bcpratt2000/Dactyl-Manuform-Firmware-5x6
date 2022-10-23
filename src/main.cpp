#include <Arduino.h>
#include <PluggableUSBHID.h>
#include <USBHID_Types.h>
#include <USBKeyboard.h>

USBKeyboard keyboard;
enum ADDITIONAL_KEY{
  KEY_ESC = 27,
  KEY_TAB = 9,
  KEY_BACKSPACE = 8,
  KEY_ENTER = 13,
  KEY_SPACE = 23
};
#define LED 25
#define SLEW_US 10  // time is microseconds to wait between setting the pin
                    // voltage to high
                    // and reading the output value

// Is this compiled for the left half or the right half
//  #define LEFT
#define RIGHT

// define the size of the keyboard
#define MATRIX_HEIGHT 6
#define MATRIX_WIDTH 6

// define history of keypresses
bool lastState[MATRIX_WIDTH][MATRIX_HEIGHT];
bool currentState[MATRIX_WIDTH][MATRIX_HEIGHT];

// define the pins used for the key matrix
short outputPins[MATRIX_HEIGHT] = {2, 3, 4, 5, 6, 7};
short inputPins[MATRIX_WIDTH] = {8, 9, 10, 11, 12, 13};

// Define Keymaps
#ifdef LEFT
short Keymap[MATRIX_WIDTH][MATRIX_HEIGHT] = {
    {KEY_ESC, '1', '2', '3', '4', '5'}, {KEY_TAB, 'q', 'w', 'e', 'r', 't'}, {KEY_SHIFT, 'a', 's', 'd', 'f', 'g'},
    {KEY_CTRL, 'z', 'x', 'c', 'v', 'b'}, {'[', ']', KEY_ENTER, KEY_SPACE, 0, 0}, {0, 0, 0, 0, 0, 0}};
#endif
#ifdef RIGHT
short Keymap[MATRIX_WIDTH][MATRIX_HEIGHT] = {
    {'6', '7', '8', '9', '0', KEY_BACKSPACE}, {'y', 'u', 'i', 'o', 'p', '-'}, {'h', 'j', 'k', 'l', ';', '\''},
    {'n', 'm', ',', '.', '/', '\\'}, {0, 0, KEY_SPACE, KEY_BACKSPACE, '=', '-'}, {0, 0, 0, 0, 0, 0}};
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
