#include "I2CKeyboard.h"

I2CKeyboard::I2CKeyboard(unsigned int bitrate, uint8_t address) {
  this->address = address;
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.setTimeout(10);
  Wire.begin(address);

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

  // start I2C listerner on second core
  // multicore_reset_core1();
  // multicore_launch_core1(secondCoreListener);
}
void I2CKeyboard::periodic() {
  int key_cursor_incrementer = 0;

  uint8_t modifier = 0;
  uint8_t keys[MAX_KEYS];

  unsigned long nextReportTime = micros();

  nextReportTime = micros() + (1000000 / PULLING_HZ);
  if (micros() >= nextReportTime) {
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
      for (int i = 0; i < MAX_KEYS; i++) {
        if (keys[i] = !0) {
          // compare both key lists and only take the first 6 keys if too
          // many are pressed
        }
      }
      modifier = peers[address].modifier | peers[2].modifier;
      keyboard.keyboardReport(0, modifier, keys);
    }
  }
}

void I2CKeyboard::sendKeys(uint8_t submittedModifier, uint8_t* submittedKeys) {
  // HID_REPORT report;
  // report.data[0] = 1;
  // report.data[1] = modifier;
  // report.data[2] = 0;

  // for (int i = 0; i < MAX_KEYS; i++) {
  //   report.data[3 + i] = keys[i];
  // }

  // report.length = 3 + MAX_KEYS;

  // keyboard.send(&report);

  peers[address].modifier = submittedModifier;
  for (int i = 0; i < MAX_KEYS; i++) {
    peers[address].keys[i] = submittedKeys[i];
  }
}
