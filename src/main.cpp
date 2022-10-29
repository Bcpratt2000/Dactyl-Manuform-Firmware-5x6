#include <Arduino.h>
#include <PluggableUSBHID.h>
#include <USBHID_Types.h>
#include <USBKeyboard.h>

USBKeyboard keyboard;
void sendKeys(uint8_t modifier, uint8_t* keys);
enum KEYS {
  KEY_A = 4,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  KEY_0,
  KEY_ENTER,
  KEY_ESC,
  KEY_BACKSPACE,
  KEY_TAB,
  KEY_SPACE,
  KEY_MINUS,
  KEY_EQUAL,
  KEY_LBRACE,
  KEY_RBRACE,
  KEY_BACKSLASH,

  KEY_SEMICOLON = 51,
  KEY_QUOTE,
  KEY_TILDE,
  KEY_COMMA,
  KEY_PERIOD,
  KEY_FORWARDSLASH,
  KEY_CAPSLOCK,
  KEY_F1,
  KEY_F2,
  KEY_F3,
  KEY_F4,
  KEY_F5,
  KEY_F6,
  KEY_F7,
  KEY_F8,
  KEY_F9,
  KEY_F10,
  KEY_F11,
  KEY_F12,
  KEY_PRINTSCREEN,
  KEY_SCROLLLOCK,
  KEY_PAUSE,
  KEY_INSERT,
  KEY_HOME,
  KEY_PAGEUP,
  KEY_DELETE,
  KEY_END,
  KEY_PAGEDOWN,
  KEY_RIGHTARROW,
  KEY_LEFTARROW,
  KEY_DOWNARROW,
  KEY_UPARROW,
  KEYPAD_NUMLOCK,
  KEYPAD_FORWARDSLASH,
  KEYPAD_ASTERISK,
  KEYPAD_MINUS,
  KEYPAD_PLUS,
  KEYPAD_ENTER,
  KEYPAD_1,
  KEYPAD_2,
  KEYPAD_3,
  KEYPAD_4,
  KEYPAD_5,
  KEYPAD_6,
  KEYPAD_7,
  KEYPAD_8,
  KEYPAD_9,
  KEYPAD_0,
  KEYPAD_PERIOD,

  KEYPAD_EQUAL = 103,

  KEY_LCTRL = 224,
  KEY_LSHIFT,
  KEY_LALT,
  KEY_LLOGO,
  KEY_RCTRL,
  KEY_RSHIFT,
  KEY_RALT,
  KEY_RLOGO
};
#define LED 25
#define MAX_KEYS 6  // maximum number of keys pressed at once
#define SLEW_US \
  5  // time is microseconds to wait between setting the pin
      // voltage to high
      // and reading the output value

// Is this compiled for the left half or the right half
// #define LEFT
#define RIGHT

// define the size of the keyboard
#define MATRIX_HEIGHT 6
#define MATRIX_WIDTH 6

// define history of keypresses
bool currentState[MATRIX_WIDTH][MATRIX_HEIGHT];

// define the pins used for the key matrix
uint8_t inputPins[MATRIX_HEIGHT] = {2, 3, 4, 5, 6, 7};
uint8_t outputPins[MATRIX_WIDTH] = {8, 9, 10, 11, 12, 13};

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
  for (int i = 0; i < MATRIX_WIDTH; i++) {
    digitalWrite(outputPins[i], HIGH);
    delayMicroseconds(SLEW_US);
    for (int j = 0; j < MATRIX_HEIGHT; j++) {
      if (digitalRead(inputPins[j]) == HIGH) {
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
  sendKeys(modifier, keyList);
}

void sendKeys(uint8_t modifier, uint8_t* keys) {
  HID_REPORT report;
  report.data[0] = 1;
  report.data[1] = modifier;
  report.data[2] = 0;

  for (int i = 0; i < MAX_KEYS; i++) {
    report.data[3 + i] = keys[i];
  }

  report.length = 3 + MAX_KEYS;

  keyboard.send(&report);
}