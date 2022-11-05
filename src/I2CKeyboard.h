#pragma once

#include <Arduino.h>
#include <PluggableUSBHID.h>
#include <USBHID_Types.h>
#include <USBKeyboard.h>
#include <Wire.h>
#include <pico/multicore.h>

#include "KEYS.h"

#define MAX_KEYS 6  // maximum number of keys that can be pressed at once

#define PULLING_HZ 1000

class I2CKeyboard {
 public:
  I2CKeyboard(unsigned int bitrate, uint8_t address);
  void sendKeys(uint8_t modifier, uint8_t* keys);
  void periodic();
  

 private:
    struct i2cPeer {
    uint16_t uniqueIdentifier;

    bool isLayerUp;
    bool isLayerDown;

    uint8_t modifier;
    uint8_t keys[MAX_KEYS];
  };

  uint8_t modifier;
  uint8_t keys[MAX_KEYS];

  USBKeyboard keyboard;
  uint8_t address;
  i2cPeer peers[128];

  // void secondCoreListener();
};