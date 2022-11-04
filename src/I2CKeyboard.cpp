#include "I2CKeyboard.h"
// #include "pico/stdlib.h"
#include "pico/multicore.h"

#define MAX_KEYS 6  // maximum number of keys that can be pressed at once

I2CKeyboard::I2CKeyboard(unsigned int bitrate, uint8_t address) {
  this->address = address;
  
  // Wire.setSCA(0);
  // Wire.setSCL(1);

  if (address == 1) {
    // initalize own peer
    peers[address].uniqueIdentifier = address;
    peers[address].modifier = 0;
    peers[address].isLayerDown = false;
    peers[address].isLayerUp = false;
    for (int i = 0; i < sizeof(peers[address].keys) / sizeof(int); i++) {
      peers[address].keys[i] = 0;
    }
  }

  // start I2C listerner on second core
  // multicore_launch_core1(secondCoreListener);
}

void secondCoreListener(){};

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
