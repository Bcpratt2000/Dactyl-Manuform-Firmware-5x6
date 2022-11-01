#include <Arduino.h>
#include <PluggableUSBHID.h>
#include <USBHID_Types.h>
#include <USBKeyboard.h>

// #ifndef KEYS
// #include "KEYS.h"
// #endif

#include "I2CKeyboard.h"


I2CKeyboard keyboard;
void sendKeys(uint8_t modifier, uint8_t* keys);


#define LED 25
#define MAX_KEYS 6  // maximum number of keys pressed at once
#define SLEW_US \
  3  // time is microseconds to wait between setting the pin
      // voltage to high
      // and reading the output value
#define DEBOUNCE_US 100 //debounce time for combating false button presses

// Is this compiled for the left half or the right half
#define LEFT
// #define RIGHT

// define the size of the keyboard
#define MATRIX_HEIGHT 6
#define MATRIX_WIDTH 6

// define history of keypresses
bool currentState[MATRIX_WIDTH][MATRIX_HEIGHT];

// define the pins used for the key matrix
uint8_t inputPins[MATRIX_HEIGHT] = {2, 3, 4, 5, 6, 7};
uint8_t outputPins[MATRIX_WIDTH] = {8, 9, 10, 11, 12, 13};

uint8_t debounceFirstCheckPresses[MATRIX_WIDTH][MATRIX_HEIGHT];
uint8_t keyList[MAX_KEYS];
uint8_t keysPressed = 0;
uint8_t modifier = 0;

// Define Keymaps
#ifdef LEFT
uint8_t Keymap[MATRIX_WIDTH][MATRIX_HEIGHT] = {
    {KEYS::KEY_ESC, KEYS::KEY_1, KEYS::KEY_2, KEYS::KEY_3, KEYS::KEY_4,
     KEYS::KEY_5},
    {KEYS::KEY_TAB, KEYS::KEY_Q, KEYS::KEY_W, KEYS::KEY_E, KEYS::KEY_R,
     KEYS::KEY_T},
    {KEYS::KEY_PAGEUP, KEYS::KEY_A, KEYS::KEY_S, KEYS::KEY_D, KEYS::KEY_F,
     KEYS::KEY_G},
    {KEYS::KEY_PAGEDOWN, KEYS::KEY_Z, KEYS::KEY_X, KEYS::KEY_C, KEYS::KEY_V,
     KEYS::KEY_B},
    {0, 0, KEYS::KEY_LBRACE, KEYS::KEY_RBRACE, KEYS::KEY_SPACE, KEYS::KEY_LSHIFT},
    {0, 0, KEYS::KEY_ENTER, KEYS::KEY_LCTRL, KEYS::KEY_LALT, KEYS::KEY_LLOGO}};
#endif
#ifdef RIGHT
uint8_t Keymap[MATRIX_WIDTH][MATRIX_HEIGHT] = {
    {KEYS::KEY_6, KEYS::KEY_7, KEYS::KEY_8, KEYS::KEY_9, KEYS::KEY_0,
     KEY_DELETE},
    {KEYS::KEY_Y, KEYS::KEY_U, KEYS::KEY_I, KEYS::KEY_O, KEYS::KEY_P,
     KEYS::KEY_MINUS},
    {KEYS::KEY_H, KEYS::KEY_J, KEYS::KEY_K, KEYS::KEY_L, KEYS::KEY_SEMICOLON,
     KEYS::KEY_QUOTE},
    {KEYS::KEY_N, KEYS::KEY_M, KEYS::KEY_COMMA, KEYS::KEY_PERIOD,
     KEYS::KEY_FORWARDSLASH, KEYS::KEY_BACKSLASH},
    {KEYS::KEY_DELETE, KEYS::KEY_SPACE, KEYS::KEY_MINUS,
     KEYS::KEY_EQUAL, 0, 0},
    {KEYS::KEY_END, KEYS::KEY_HOME, 0, KEYS::KEY_BACKSPACE, 0, 0}};
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

  //wait for keyboard ready
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);

}

// Entry Point
void loop() {

  // zero keys pressed array
  for (int i = 0; i < MAX_KEYS; i++) {
    keyList[i] = 0;
  }

  // zero counter of keys pressed and modifier bitmask
  keysPressed = 0;
  modifier = 0;


  
  // loop through button matrix and fill the list of keys pressed
  // for (int i = 0; i < MATRIX_WIDTH; i++) {
  //   digitalWrite(outputPins[i], HIGH);
  //   delayMicroseconds(SLEW_US);
  //   for (int j = 0; j < MATRIX_HEIGHT; j++) {
  //     if (digitalRead(inputPins[j]) == HIGH) {
  //       if (Keymap[i][j] == KEYS::KEY_LCTRL) {
  //         modifier |= 1;
  //       } else if (Keymap[i][j] == KEYS::KEY_LSHIFT) {
  //         modifier |= 1 << 1;
  //       } else if (Keymap[i][j] == KEYS::KEY_LALT) {
  //         modifier |= 1 << 2;
  //       } else if (Keymap[i][j] == KEYS::KEY_LLOGO) {
  //         modifier |= 1 << 3;
  //       } else if (Keymap[i][j] == KEYS::KEY_RCTRL) {
  //         modifier |= 1 << 4;
  //       } else if (Keymap[i][j] == KEYS::KEY_RSHIFT) {
  //         modifier |= 1 << 5;
  //       } else if (Keymap[i][j] == KEYS::KEY_RALT) {
  //         modifier |= 1 << 6;
  //       } else if (Keymap[i][j] == KEYS::KEY_RLOGO) {
  //         modifier |= 1 << 7;
  //       } else {
  //         if (keysPressed < MAX_KEYS) {
  //           keyList[keysPressed] = Keymap[i][j];
  //           keysPressed++;
  //         }
  //       }
  //     }
  //   }
  //   digitalWrite(outputPins[i], LOW);
  // }

//loop through button array and send the button presses to the next part through the debounceFirstCheckPresses array
for (int i = 0; i < MATRIX_WIDTH; i++) {
    digitalWrite(outputPins[i], HIGH);
    delayMicroseconds(SLEW_US);
    for (int j = 0; j < MATRIX_HEIGHT; j++) {
      if (digitalRead(inputPins[j]) == HIGH) {
        debounceFirstCheckPresses[i][j] = 1;
      } else{
        debounceFirstCheckPresses[i][j] = 0;
      }
    }
    digitalWrite(outputPins[i], LOW);
}

delayMicroseconds(DEBOUNCE_US);
  // loop through the debounce matrix and trigger the actual button presses
  for (int i = 0; i < MATRIX_WIDTH; i++) {
    digitalWrite(outputPins[i], HIGH);
    delayMicroseconds(SLEW_US);
    for (int j = 0; j < MATRIX_HEIGHT; j++) {
      if (digitalRead(inputPins[j]) && debounceFirstCheckPresses[i][j]) {
        if (Keymap[i][j] == KEYS::KEY_LCTRL) {
          modifier |= 1;
        } else if (Keymap[i][j] == KEYS::KEY_LSHIFT) {
          modifier |= 1 << 1;
        } else if (Keymap[i][j] == KEYS::KEY_LALT) {
          modifier |= 1 << 2;
        } else if (Keymap[i][j] == KEYS::KEY_LLOGO) {
          modifier |= 1 << 3;
        } else if (Keymap[i][j] == KEYS::KEY_RCTRL) {
          modifier |= 1 << 4;
        } else if (Keymap[i][j] == KEYS::KEY_RSHIFT) {
          modifier |= 1 << 5;
        } else if (Keymap[i][j] == KEYS::KEY_RALT) {
          modifier |= 1 << 6;
        } else if (Keymap[i][j] == KEYS::KEY_RLOGO) {
          modifier |= 1 << 7;
        } else {
          if (keysPressed < MAX_KEYS) {
            keyList[keysPressed] = Keymap[i][j];
            keysPressed++;
          }
        }
      }
    }
    digitalWrite(outputPins[i], LOW);
  }

  if(keysPressed){
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  keyboard.sendKeys(modifier, keyList);
}




// void sendKeys(uint8_t modifier, uint8_t* keys) {
//   HID_REPORT report;
//   report.data[0] = 1;
//   report.data[1] = modifier;
//   report.data[2] = 0;

//   for (int i = 0; i < MAX_KEYS; i++) {
//     report.data[3 + i] = keys[i];
//   }

//   report.length = 3 + MAX_KEYS;

//   keyboard.send(&report);
// }