#include <ArduinoJson.h>
#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN 13
DHT dht(DHTPIN, DHTTYPE);

void tempJson(float n){
  StaticJsonDocument<48> doc;
  doc["sensor"] = "DHT11";
  doc["temp"] = n;
  serializeJson(doc, Serial);
  Serial.println();
}

void humdJson(float n){
  StaticJsonDocument<48> doc;
  doc["sensor"] = "DHT11";
  doc["humd"] = n;
  serializeJson(doc, Serial);
  Serial.println();
}

void setup() {
  dht.begin();
  Serial.begin(115200);
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if(isnan(t) || isnan(h)){
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  tempJson(t);
  humdJson(h);
  delay(3000);
  
}
