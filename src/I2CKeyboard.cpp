#include "I2CKeyboard.h"

#define MAX_KEYS 6  // maximum number of keys that can be pressed at once

I2CKeyboard::I2CKeyboard(unsigned int bitrate, uint8_t address){
  this->address = address;
  // Wire.begin(bitrate);
  
}

void I2CKeyboard::sendKeys(uint8_t modifier, uint8_t* keys) {
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
