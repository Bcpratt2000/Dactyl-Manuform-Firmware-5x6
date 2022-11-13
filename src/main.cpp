#include <Arduino.h>

#include "I2CKeyboard.h"
#include "KEYS.h"
// #include "arduino/hid/Adafruit_USBD_HID.h"

// Include the header file with all of the configuration options
// #include "Configurations/Dactyl-Manuform/Dactyl-Manuform_Left.h"
#include "Configurations/Dactyl-Manuform/Dactyl-Manuform_Right.h"

// define history of keypresses
bool currentState[MATRIX_WIDTH][MATRIX_HEIGHT];

uint8_t debounceFirstCheckPresses[MATRIX_WIDTH][MATRIX_HEIGHT];
uint8_t keyList[MAX_KEYS];
uint8_t keysPressed = 0;
uint8_t modifier = 0;
bool layerUpState = false;
bool layerDownState = false;
bool layerUpPressedThisRound = false;
bool layerDownPressedThisRound = false;

// Adafruit_USBD_HID keyboard;
I2CKeyboard keyboard;
// USBKeyboard keyboard;
void sendKeys(uint8_t modifier, uint8_t* keys);
void requestEvent();

uint8_t const desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(),
};

#define LED 25
#define MAX_KEYS 6  // maximum number of keys pressed at once
#define SLEW_US \
  5                      // time is microseconds to wait between setting the pin
                         // voltage to high
                         // and reading the output value
#define DEBOUNCE_US 300  // debounce time for combating false button presses

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  // TinyUSB requires to run this in order to allow the computer to reset the
  // pico for uploading code
  Serial.begin(115200);

  // set output and input pinmodes
  for (int i = 0; i < MATRIX_HEIGHT; i++) {
    pinMode(outputPins[i], OUTPUT);
  }
  for (int i = 0; i < MATRIX_WIDTH; i++) {
    pinMode(inputPins[i], INPUT_PULLDOWN);
  }
  keyboard.begin(PIN_SDA, PIN_SCL, 50000, I2C_ADDRESS);
  Wire.onRequest(requestEvent);
  digitalWrite(LED, LOW);
  delay(100);
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
  layerUpPressedThisRound = false;
  layerDownPressedThisRound = false;
  // loop through button array and send the button presses to the next part
  // through the debounceFirstCheckPresses array
  for (int i = 0; i < MATRIX_WIDTH; i++) {
    digitalWrite(outputPins[i], HIGH);
    delayMicroseconds(SLEW_US);
    for (int j = 0; j < MATRIX_HEIGHT; j++) {
      if (digitalRead(inputPins[j]) == HIGH) {
        debounceFirstCheckPresses[i][j] = 1;
      } else {
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
        } else if (Keymap[i][j] == KEYS::LAYER_UP) {
          layerUpPressedThisRound = true;
        } else if (Keymap[i][j] == KEYS::LAYER_DOWN) {
          layerDownPressedThisRound = true;
        } else {
          if (keysPressed < MAX_KEYS) {
            if (layerUpState) {
              keyList[keysPressed] = Keymap_Layer_Up[i][j];
            } else if (layerDownState) {
              keyList[keysPressed] = Keymap_Layer_Down[i][j];
            } else {
              keyList[keysPressed] = Keymap[i][j];
            }
            keysPressed++;
          }
        }
      }
    }
    digitalWrite(outputPins[i], LOW);
  }
  if (layerUpPressedThisRound) {
    layerUpState = true;
  } else {
    layerUpState = false;
  }
  if (layerDownPressedThisRound) {
    layerDownState = true;
  } else {
    layerDownState = false;
  }
  keyboard.sendKeys(modifier, keyList);
  keyboard.periodic();
}

void requestEvent() {
  digitalWrite(LED, HIGH);
  // reply to request with the modifier and then an array of keys
  Wire.write(keyboard.peers[keyboard.address].modifier);
  Wire.write(keyboard.peers[keyboard.address].keys, MAX_KEYS);
}