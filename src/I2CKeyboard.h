#pragma once

#include <Arduino.h>
#include <PluggableUSBHID.h>
#include <USBHID_Types.h>
#include <USBKeyboard.h>
#include <Wire.h>

#include "KEYS.h"

#define MAX_KEYS 6 //maximum number of keys that can be pressed at once

class I2CKeyboard {
 public:
  I2CKeyboard(unsigned int bitrate);
  void sendKeys(uint8_t modifier, uint8_t* keys);


 private:
  USBKeyboard keyboard;

  struct {
    uint16_t uniqueIdentifier;
    uint8_t address;
    uint8_t modifier;
    uint8_t keys[MAX_KEYS];
  } i2cPeer;
};