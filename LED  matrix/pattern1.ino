#include<LedControl.h>
int DIN = 11;
int CS = 7;
int CLK = 13;
LedControl lc = LedControl(DIN, CLK, CS, 1);
byte smile[8]={0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};
byte alien1[8]={0x42,0x24,0x7E,0xDB,0xFF,0xBD,0xA5,0x18};
byte alien2[8]={0x18,0x3C,0x7E,0xDB,0xFF,0x24,0x5A,0xA5};
byte cat[8]={B10001000,B11111000,B10101000,B01110001,B00100001,B01111001,B01111101,B10111110};

void Display(byte graph[8]) {
  for(int i=0;i<8;i++) lc.setRow(0,i,graph[i]);
  delay(5000);
  lc.clearDisplay(0);
}
void setup() {
  lc.shutdown(0,false); //設定為 true，設備進入電力中斷模式，設定為 false 為正常模式
  lc.setIntensity(0,0);
  lc.clearDisplay(0);
}

void loop() {
  Display(smile);
  Display(alien1);
  Display(alien2);
  Display(cat);
}
