#include <Arduino.h>

#include "KEYS.h"

// define the size of the keyboard
#define MATRIX_HEIGHT 6
#define MATRIX_WIDTH 6

#define I2C_ADDRESS 10

#define PIN_SDA 16
#define PIN_SCL 17

// define the pins used for the key matrix
uint8_t inputPins[MATRIX_HEIGHT] = {2, 3, 4, 5, 6, 7};
uint8_t outputPins[MATRIX_WIDTH] = {8, 9, 10, 11, 12, 13};

uint8_t Keymap[MATRIX_WIDTH][MATRIX_HEIGHT] = {
    {KEYS::KEY_ESC, KEYS::KEY_1, KEYS::KEY_2, KEYS::KEY_3, KEYS::KEY_4,
     KEYS::KEY_5},
    {KEYS::KEY_TAB, KEYS::KEY_Q, KEYS::KEY_W, KEYS::KEY_E, KEYS::KEY_R,
     KEYS::KEY_T},
    {KEYS::KEY_PAGEUP, KEYS::KEY_A, KEYS::KEY_S, KEYS::KEY_D, KEYS::KEY_F,
     KEYS::KEY_G},
    {KEYS::KEY_PAGEDOWN, KEYS::KEY_Z, KEYS::KEY_X, KEYS::KEY_C, KEYS::KEY_V,
     KEYS::KEY_B},
    {0, 0, KEYS::KEY_LBRACE, KEYS::KEY_RBRACE, KEYS::KEY_SPACE,
     KEYS::KEY_LSHIFT},
    {0, 0, KEYS::KEY_ENTER, KEYS::KEY_LCTRL, KEYS::KEY_LALT, KEYS::KEY_LLOGO}};

uint8_t Keymap_Layer_Up[MATRIX_WIDTH][MATRIX_HEIGHT] = {
    {KEYS::KEY_TILDE, KEYS::KEY_F1, KEYS::KEY_F2, KEYS::KEY_F3, KEYS::KEY_F4,
     KEYS::KEY_F5},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}};

uint8_t Keymap_Layer_Down[MATRIX_WIDTH][MATRIX_HEIGHT] = {
    {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
