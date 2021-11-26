#include<LedControl.h>
int DIN = 11;
int CS = 7;
int CLK = 13;
LedControl lc = LedControl(DIN, CLK, CS, 0);
byte heart[8]={0x66,0xFF,0xDB,0xFF,0xDB,0x66,0x3C,0x18};


void setup() {
  lc.shutdown(0,false); 
  lc.setIntensity(0,0);
}

void loop() {
  for(int i=0;i<8;i++) lc.setRow(0,i,heart[i]);
  lc.setIntensity(0,5);
  delay(800);
  lc.setIntensity(0,0);
  delay(500);  
}
