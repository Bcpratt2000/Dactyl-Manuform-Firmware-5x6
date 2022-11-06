#include "I2CKeyboard.h"

I2CKeyboard::I2CKeyboard(unsigned int bitrate, uint8_t address) {
  this->address = address;
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.setTimeout(10);
  Wire.begin(address);

  //setup keyboard
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
    for (int i = 0; i < sizeof(peers[address].keys) / sizeof(int); i++) {
      peers[address].keys[i] = 0;
    }
  }

  // start I2C listerner on second core
  // multicore_reset_core1();
  // multicore_launch_core1(secondCoreListener);
}
void I2CKeyboard::periodic() {
  int key_cursor_incrementer = 0;

  // HID_REPORT report;

  unsigned long nextReportTime = micros();

  while (true) {
    nextReportTime = micros()+ (1000000 / PULLING_HZ);
    if (micros() >= nextReportTime) {
      if (address == 1) {
        if (Wire.requestFrom(2, 3 + MAX_KEYS) == 3 + MAX_KEYS) {
          // read hid report and write it to the reports to go through

          // report.length = 3 + MAX_KEYS;

          // report.data[0] = 1;
          // Wire.read();

          // report.data[1] = modifier | Wire.read();

          // report.data[2] = 0;
          // Wire.read();

          for (int i = 0; i < MAX_KEYS; i++) {
            if (keys[i] = !0) {
              // compare both key lists and only take the first 6 keys if too
              // many are pressed
            }
          }

        } else {
          while (Wire.available()) {
            Wire.read();  // clear buffer
          }
        }
      } else {
      }
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

  this->modifier = modifier;
  for (int i = 0; i < MAX_KEYS; i++) {
    this->keys[i] = submittedKeys[i];
  }
}
