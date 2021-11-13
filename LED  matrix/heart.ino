#include <LedControl.h>
#define ldrPin A0
int DIN = 12;
int CS = 11;
int CLK = 10;
int ldrValue = 0;

LedControl lc = LedControl(DIN,CS,CLK,0);

void setup(){
  lc.shutdown(0,false);
  lc.setIntensity(0,15);
  lc.clearDisplay(0);
  ldrValue = 0.5*ldrValue + 0.5*analogRead(ldrPin); //與前次的讀取數值相加取平均，為了減少誤差
}
  
void loop(){
  
