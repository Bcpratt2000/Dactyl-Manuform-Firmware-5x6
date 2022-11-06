#include <Arduino.h>
#include "I2CKeyboard.h"
#include "KEYS.h"
#include "arduino/hid/Adafruit_USBD_HID.h"

// Include the header file with all of the configuration options
//  #include "Configurations/Dactyl-Manuform/Dactyl-Manuform_Left.h"
#include "Configurations/Dactyl-Manuform/Dactyl-Manuform_Right.h"

// define history of keypresses
bool currentState[MATRIX_WIDTH][MATRIX_HEIGHT];

uint8_t debounceFirstCheckPresses[MATRIX_WIDTH][MATRIX_HEIGHT];
uint8_t keyList[MAX_KEYS];
uint8_t keysPressed = 0;
uint8_t modifier = 0;

Adafruit_USBD_HID keyboard;
// I2CKeyboard keyboard(115200, I2C_ADDRESS);
// USBKeyboard keyboard;
void sendKeys(uint8_t modifier, uint8_t* keys);

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
  //TinyUSB requires to run this in order to allow the computer to reset the pico for uploading code
  Serial.begin(115200);


  keyboard.setPollInterval(2);
  keyboard.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  // keyboard.setReportCallback(NULL, hid_report_callback);
  keyboard.begin();

  pinMode(LED, OUTPUT);

  // set output and input pinmodes
  for (int i = 0; i < MATRIX_HEIGHT; i++) {
    pinMode(outputPins[i], OUTPUT);
  }
  for (int i = 0; i < MATRIX_WIDTH; i++) {
    pinMode(inputPins[i], INPUT_PULLDOWN);
  }

  // wait for keyboard ready
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

  if (keysPressed) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  // keyboard.sendKeys(modifier, keyList);
  sendKeys(modifier, keyList);
}

void sendKeys(uint8_t modifier, uint8_t* keys) {
  keyboard.keyboardReport(0, modifier, keys);
}
