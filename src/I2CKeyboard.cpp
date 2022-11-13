#include "I2CKeyboard.h"

I2CKeyboard::I2CKeyboard(){};

void I2CKeyboard::begin(unsigned int pinSDA, unsigned int pinSCL, unsigned int bitrate, uint8_t passedAddress) {
  digitalWrite(25, HIGH);
  address = passedAddress;
  Wire.setSDA(pinSDA);
  Wire.setSCL(pinSCL);
  Wire.setTimeout(10);
  Wire.setClock(bitrate);
  if(address == 9){
    Wire.begin();
  } else{
    Wire.begin(address);
  }

  // setup keyboard
  uint8_t desc_hid_report[] = {
      TUD_HID_REPORT_DESC_KEYBOARD(),
  };


  keyboard.setPollInterval(2);
  keyboard.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  // keyboard.setReportCallback(NULL, hid_report_callback);
  keyboard.begin();

  // initalize own peer
  peers[address].uniqueIdentifier = address;
  peers[address].modifier = 0;
  peers[address].isLayerDown = false;
  peers[address].isLayerUp = false;
  for (int i = 0; i < MAX_KEYS; i++) {
    peers[address].keys[i] = 0;
  }

  nextReportTime = micros();
  delay(1000);

  // Wire.onRequest(onRequest);
  digitalWrite(25, LOW);
}

void I2CKeyboard::periodic() {
  if (micros() >= nextReportTime) {
    // Wire.beginTransmission(10);
    // Wire.write("Hello, World");
    // Wire.endTransmission();
    nextReportTime = micros() + (1000000 / PULLING_HZ);
    uint8_t modifier = 0;
    uint8_t keys[MAX_KEYS] = {0};

    // if this is the host device (address of 9), request the data
    // from 10 and package it all up as an hid report and send it
    if (address == 9) {
      if (Wire.requestFrom(10, 1 + MAX_KEYS) == 1 + MAX_KEYS) {
        // read hid report and write it to the reports to go through
        peers[10].modifier = Wire.read();

        for (int i = 0; i < MAX_KEYS; i++) {
          peers[10].keys[i] = Wire.read();
        }

      } else {
        while (Wire.available()) {
          Wire.read();  // clear buffer
        }
      }

      // compare both key lists and only take the first MAX_KEYS if too many
      // are pressed
      int key_cursor_incrementer = 0;
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < MAX_KEYS; j++) {
          if (peers[i + 9].keys[j] != 0) {
            if (key_cursor_incrementer < MAX_KEYS) {
              keys[key_cursor_incrementer] = peers[i + 9].keys[j];
              key_cursor_incrementer++;
            }
          }
        }
      }
      // turn LED on if any keys are pressed
      if (key_cursor_incrementer) {
        digitalWrite(25, HIGH);
      } else {
        digitalWrite(25, LOW);
      }
      modifier = peers[address].modifier | peers[10].modifier;
      keyboard.keyboardReport(0, modifier, keys);
    }
  }
}

void I2CKeyboard::sendKeys(uint8_t submittedModifier, uint8_t* submittedKeys) {
  peers[address].modifier = submittedModifier;
  for (int i = 0; i < MAX_KEYS; i++) {
    peers[address].keys[i] = submittedKeys[i];
  }
}
