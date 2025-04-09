#include "WiFi.h"
#include "ESPAsyncWebServer.h"

const char* ssid = "ssid";
const char* password = "pass";

const int ledPin = 2;
bool ledState = false;

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Async Server</title>
</head>
<body>
  <h1>ESP32 LED Control</h1>
  <p>LED State: <strong>%STATE%</strong></p>
  <p><a href="/toggle"><button>Toggle LED</button></a></p>
</body>
</html>
)rawliteral";

String processor(const String& var) {
  if (var == "STATE") {
    return ledState ? "ON" : "OFF";
  }
  return String();
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request){
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    request->redirect("/");
  });

  
  server.begin();
}

void loop() {
  
}
