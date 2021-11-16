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

AsyncWebServer server(80); //建立HTTP伺服器物件
AsyncWebSocket ws("/ws"); //建立WebSocket物件，/ws為自訂的路徑

//Web Page(html, css, javascript)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    h1{
      font-size: 1.8rem;
      color: white;
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
  //window.location.hostname抓取目前網頁IP位址
  var gateway =`ws://${window.location.hostname}/ws`; //It is the entry point to the WebSocket interface
  var websocket;
  function initWebSocket() { //初始化前面所定義閘道器(gateway)的WebSocket connection，並指定數個回呼函數
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen; //當WebSocket connection開通，關閉或訊息接收
    websocket.onclose = onClose;
    websocket.onmessage = onMessage; 
  }
  function onOpen(event) {
    console.log('Connection opened');
    websocket.send('Hi'); //為了確認連結是否初始化
  }  
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000); //當連接意外斷開時，過2秒後再呼叫initWebSocket()
  }
  function onMessage(event) {
    var state;
    if(event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
    document.getElementById('state').innerHTML = state;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() { //When you click the button, the toggle function is called.
    document.getElementById('button').addEventListener('click',toggle);
  }
  function toggle() { //透過WebSocket connection傳送帶有"toggle"文本的訊息
    websocket.send('toggle');
  }
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

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, unit8_t *data, size_t len){
  //接收WebSocket資料的程式
  switch(type) {
    case WS_EVT_CONNECT: //有個新用戶連接
      //從client物件取得用戶端IP位址(remoteIP)和編號(id)
      Serial.printf("WebSocket client #%u connected form %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT: //有個用戶端離線
      Serial.printf("用戶%u 已離線\n", client->id());
      break;
    case WS_EVT_DATA: //用戶端傳入資料
      Serial.printf("用戶%u 傳入資料: %s\n", client->id(), (char *)data);
      break;
    case WS_EVT_PONG: //回應ping請求
    case WS_EVT_ERROR: //收到錯誤訊息
      break;
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
  ws.cleanupClients(); //清理用戶端
  digitalWrite(ledPin, ledState);
}
  
  
