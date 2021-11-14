#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h> //提供WebSocket插件
#include <DHT.h>

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

bool ledState = 0;
const int ledPin =2;

AsyncWebServer server(80); //建立AsyncWebServer物件在80埠
AsyncWebSocket ws("/ws"); //建立AsyncWebSocket物件名為ws，處理/ws路徑上的連接

//Web Page(html, css, javascript)
const char index_html[] PROGMEM = R"rawliteral(
)rawliteral";

//Notify All Clients
void notifyClients() {
  ws.textAll(String(ledState)); //textAll()同時傳送相同訊息給所有用戶端
}

//回呼函數作用為透過WebSocket協定接收來自用戶端新訊息
void handleWebSocketMessage(void *arg, unit8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if(info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if(strcmp(char*)data, "toggle") == 0) {
      ledState =! ledState;
      notifyClients();
    }
  }
}
  
