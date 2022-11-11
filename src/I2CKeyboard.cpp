#include "I2CKeyboard.h"

I2CKeyboard::I2CKeyboard(){};

void I2CKeyboard::begin(unsigned int bitrate, uint8_t address) {
  digitalWrite(25, HIGH);
  this->address = address;
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.setTimeout(1);
  Wire.setClock(bitrate);
  if (address == 1) {
    Wire.begin();
  } else {
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

  if (address == 1) {
    // initalize own peer
    peers[address].uniqueIdentifier = address;
    peers[address].modifier = 0;
    peers[address].isLayerDown = false;
    peers[address].isLayerUp = false;
    for (int i = 0; i < MAX_KEYS; i++) {
      peers[address].keys[i] = 0;
    }
  }

  nextReportTime = micros();
  delay(1000);

  // Wire.onRequest(onRequest);
  digitalWrite(25, LOW);
}

void I2CKeyboard::periodic() {
  if (micros() >= nextReportTime) {
    // Wire.printf("Hello, World!");
    nextReportTime = micros() + (1000000 / PULLING_HZ);
    uint8_t modifier = 0;
    uint8_t keys[MAX_KEYS] = {0};

    // if this is the host device (address of 1), request the data
    // from 2 and package it all up as an hid report and send it
    if (address == 1) {
      if (Wire.requestFrom(2, 1 + MAX_KEYS) == 1 + MAX_KEYS) {
        // read hid report and write it to the reports to go through

        peers[2].modifier = Wire.read();

        for (int i = 0; i < MAX_KEYS; i++) {
          peers[2].keys[i] = Wire.read();
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
          if (peers[i + 1].keys[j] != 0) {
            if (key_cursor_incrementer < MAX_KEYS) {
              keys[key_cursor_incrementer] = peers[i + 1].keys[j];
              key_cursor_incrementer++;
            }
          }
        }
      }
      //turn LED on if any keys are pressed
      if (key_cursor_incrementer) {
        digitalWrite(25, HIGH);
      } else {
        digitalWrite(25, LOW);
      }
      modifier = peers[address].modifier | peers[2].modifier;
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
