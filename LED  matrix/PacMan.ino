#include <LedControl.h>
LedControl display=LedControl(12,11,10,1); //DIN=12, CLK=11, CS=10, use 1 max7219

void setup()
{
  display.shutdown(0,false); // turn off power saving, enables display
  display.setIntensity(0,1); // sets brightness (0~15 possible values)
  display.clearDisplay(0); // clear screen
}
const uint64_t IMAGES[] = { //Ghost image
  0xdbffffc3ffdb7e3c,
  0xb6fefe86feb6fc78,
  0x6cfcfc0cfc6cf8f0,
  0xd8f8f818f8d8f0e0,
  0xb0f0f030f0b0e0c0,
  0x60e0e060e060c080,
  0xc0c0c0c0c0c08000,
  0x8080808080800000,
  0x0101010101010000,
  0x0303030303030100,
  0x0607070607060301,
  0x0c0f0f0c0f0d0703,
  0x191f1f181f1b0f07,
  0x333f3f303f361f0f,
  0x667f7f617f6d3f1e
};
const int IMAGES_LEN = sizeof(IMAGES)/8;

void displayImage(uint64_t image) {
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      display.setLed(0, i, j, bitRead(row, j));
    }
  }
}

int i = 0;

void loop() {
  displayImage(IMAGES[i]);
  if (++i >= IMAGES_LEN ) {
    i = 0;
  }
  delay(300);
}
