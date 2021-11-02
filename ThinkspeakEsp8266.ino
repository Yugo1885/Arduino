#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
//WiFi
const char* ssid="48y104298";
const char* password="048790627";

WiFiClient client;

//ThingSpeak
unsigned long myChannelNumber=1;
const char* myWriteAPIKey="GXIRRTPJ1C0LLRVJ";

//Time variables
unsigned long lasttime=0;
unsigned long timerDelay=5000; //publishinig readings every 5s



void setup(){
  Serial.begin(115200);
  //Set the ESP8266 as a WiFi station
  WiFi.mode(WIFI_STA);
  //Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop(){
  //Connect or reconnect WiFi
  if(WiFi.status()!=WL_CONNECTED){
    Serial.print("Attempting to connect");
    while(WiFi.status()!=WL_CONNECTED){
      WiFi.begin(ssid,password);
      delay(5000);
     }
     Serial.println("\nConnected.");
    }
    
//Get a new reading
int photocellVal=analogRead(2);
Serial.print("阻值:");
Serial.println(photocellVal);

ThingSpeak.setField(1, photocellVal);

int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lasttime = millis();
}

