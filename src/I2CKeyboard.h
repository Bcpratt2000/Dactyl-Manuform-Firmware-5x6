#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <pico/multicore.h>

#include "KEYS.h"
#include "arduino/hid/Adafruit_USBD_HID.h"

#define MAX_KEYS 6  // maximum number of keys that can be pressed at once

#define PULLING_HZ 100

class I2CKeyboard {
 public:
  struct i2cPeer {
    uint16_t uniqueIdentifier;

    bool isLayerUp;
    bool isLayerDown;

    uint8_t modifier;
    uint8_t keys[MAX_KEYS];
  };
  void begin(unsigned int bitrate, uint8_t address);
  I2CKeyboard();
  void sendKeys(uint8_t modifier, uint8_t* keys);
  void periodic();
  uint8_t address;
  i2cPeer peers[128];

 private:
  unsigned long nextReportTime;

  Adafruit_USBD_HID keyboard;
};