#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
const int maxNums = 30;
uint16_t black = tft.color565(0,0,0);
uint16_t darkGreen = tft.color565(90,240,10);
int x[maxNums];
int y[maxNums];
char randomC[maxNums];
int dropSpeed[maxNums];

char matrix[] = "Wake up, Neo...\nThe Matrix has you...\nFollow the white rabbit.\n\nKnock, knock, Neo.";
int len = strlen(matrix);
void setup() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(black);
  tft.setTextSize(1);
  tft.setTextColor(darkGreen);
  tft.setCursor(0, 10);
  for(int i=0;i<len;i++){
    tft.print(matrix[i]);
    delay(300);
  }
  delay(2000);
  tft.fillScreen(black);
  
  randomSeed(analogRead(0));
}

void loop() {
  tft.fillScreen(black);

  for(int i=0;i<maxNums;i++){
    tft.setCursor(x[i], y[i]);
    tft.println(randomC[i]);
    dropSpeed[i] = random(1, 5);
    y[i] = y[i]+dropSpeed[i];
    if(y[i]>tft.height()){
      x[i] = random(0, tft.width());
      y[i] =-random(0, tft.height());
      randomC[i] = char(random(32, 126));
    }
  }  
  delay(100);
}
