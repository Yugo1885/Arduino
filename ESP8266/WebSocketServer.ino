#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h> //提供WebSocket插件
#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float t=0.0;
float h=0.0;

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

bool ledState = 0;
const int ledPin =2;

AsyncWebServer server(80); //建立AsyncWebServer物件在80埠
AsyncWebSocket ws("/ws"); //建立AsyncWebSocket物件名為ws，處理/ws路徑上的連接

//Web Page(html, css, javascript)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    h2{
      font-size: 1.5rem;
      font-weight: bold;
      color: #143642;
    }
  </style>
</head>
<body>
  <div>
    <h1>ESP WebSocket Server</h1>
    <div class="content">
  </div> 
  
  <div>
    <h2>OUtput -GPIO 2</h2>
    <p>state:<span id="state">%STATE%</span></p>
    <p><button id="button" class="button">Toggle</button></p>
  </div>
  
  <div>
    <span>Temperature</span>
    <span id="temperature">%TEMPERATURE%</span>
  </div>
  
  <div>
    <span>Humudity</span>
    <span id="humudity>%HUMUDITY%</span>
  </div>
</body>
<script>
  var websocket;
</script>
</html>
)rawliteral";

//Notify All Clients
void notifyClients() {
  ws.textAll(String(ledState)); //textAll()同時傳送相同訊息給所有用戶端
}

//WebSocket回呼函數，作用為透過WebSocket協定接收來自用戶端新訊息
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

void onEvent(){
}

void initWebSocket() {
}

String processor(){
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  pinMode(ledPin, LOW);
  digitalWrite(ledPin, LOW);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while(WiFi.status()!=WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  
  Serial.print("ESP Local IP Address: ");
  Serial.println(WiFi.localIP());
  
  initWebSocket(); //初始化WebSocket協定
  
  //Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  
  //Start server
  server.begin();
}

void loop() {
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
}
  
  
