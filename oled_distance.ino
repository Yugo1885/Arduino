#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define trigPin 5
#define echoPin 6
 
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
 
void setup() {
Serial.begin (9600);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
display.clearDisplay();
 
}
 
void loop() {
float duration;
float distance_cm;


digitalWrite(trigPin, LOW); //PULSE
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
 
duration = pulseIn(echoPin, HIGH);
 
distance_cm = (duration/2) / 29.1; //聲波距離公式
 
display.setCursor(10,10); //oled display
display.setTextSize(2);
display.setTextColor(WHITE);
display.println(distance_cm);
display.setCursor(90,10);
display.setTextSize(2);
display.println("cm");
display.display();
 
delay(500);
display.clearDisplay();

 
Serial.println(distance_cm);

}
