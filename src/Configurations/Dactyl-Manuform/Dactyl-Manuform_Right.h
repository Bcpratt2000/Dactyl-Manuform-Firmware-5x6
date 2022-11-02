#include <Arduino.h>

#include "KEYS.h"

// define the size of the keyboard
#define MATRIX_HEIGHT 6
#define MATRIX_WIDTH 6

#define I2C_ADDRESS 3

// define the pins used for the key matrix
uint8_t inputPins[MATRIX_HEIGHT] = {2, 3, 4, 5, 6, 7};
uint8_t outputPins[MATRIX_WIDTH] = {8, 9, 10, 11, 12, 13};

uint8_t Keymap[MATRIX_WIDTH][MATRIX_HEIGHT] = {
    {KEYS::KEY_6, KEYS::KEY_7, KEYS::KEY_8, KEYS::KEY_9, KEYS::KEY_0,
     KEY_DELETE},
    {KEYS::KEY_Y, KEYS::KEY_U, KEYS::KEY_I, KEYS::KEY_O, KEYS::KEY_P,
     KEYS::KEY_MINUS},
    {KEYS::KEY_H, KEYS::KEY_J, KEYS::KEY_K, KEYS::KEY_L, KEYS::KEY_SEMICOLON,
     KEYS::KEY_QUOTE},
    {KEYS::KEY_N, KEYS::KEY_M, KEYS::KEY_COMMA, KEYS::KEY_PERIOD,
     KEYS::KEY_FORWARDSLASH, KEYS::KEY_BACKSLASH},
    {KEYS::KEY_DELETE, KEYS::KEY_SPACE, KEYS::KEY_MINUS, KEYS::KEY_EQUAL, 0, 0},
    {KEYS::KEY_END, KEYS::KEY_HOME, KEYS::LAYER_UP, KEYS::KEY_BACKSPACE, 0, 0}};

uint8_t Keymap_Layer_Up[MATRIX_WIDTH][MATRIX_HEIGHT] = {
    {KEYS::KEY_F6, KEYS::KEY_F7, KEYS::KEY_F8, KEYS::KEY_F9, KEYS::KEY_F10,
     KEYS::KEY_F11},
    {0, 0, 0, 0, 0, 0},
    {KEYS::KEY_LEFTARROW, KEYS::KEY_DOWNARROW, KEYS::KEY_UPARROW,
     KEYS::KEY_RIGHTARROW, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}};

uint8_t Keymap_Layer_Down[MATRIX_WIDTH][MATRIX_HEIGHT] = {
    {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};