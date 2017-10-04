#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "...";
const char* password = "...";
const char* mqtt_server = "192.168.1.190"; //Сервер MQTT

IPAddress ip(192,168,1,54); //IP модуля
IPAddress gateway(192,168,1,1); // шлюз
IPAddress subnet(255,255,255,0); // маска

WiFiClient espClient;
PubSubClient client(espClient);

#define ID_CONNECT "cupboard"
#define LED     13
#define REL     12
#define BTN     0
int a = 0;

long previousMillis = 0;

void setup_wifi() {
  delay(10);
  int cn = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    cn++;
    delay(100);
    digitalWrite(LED, !digitalRead(LED));
    if (cn > 200){ESP.restart();}
  }
  digitalWrite(LED, LOW);
}

void reconnect() {
  digitalWrite(LED, !digitalRead(LED));
  while (!client.connected()) {
    a++;
    digitalWrite(LED, !digitalRead(LED));
    if (client.connect(ID_CONNECT)) {
      client.publish("myhome/Cupboard/connection", "true");
      client.publish("myhome/Cupboard/relay", "false");
      client.subscribe("myhome/Cupboard/#");
      digitalWrite(LED, HIGH);
      a = 0;
    } else {
      delay(5000);
    }
    if (a > 10){ESP.restart();}
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  String strTopic = String(topic);
  String strPayload = String((char*)payload);
  ///////////
  if (strTopic == "myhome/Cupboard/relay"){
    if (strPayload == "true"){
        digitalWrite(REL, HIGH);
    } else if(strPayload == "false"){
        digitalWrite(REL, LOW);
    }
    client.publish("myhome/Cupboard/relay", IntToBool(digitalRead(REL)));
  }
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(REL, OUTPUT);
  pinMode(BTN, INPUT);
  digitalWrite(LED, HIGH);
  Serial.begin(19200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  ArduinoOTA.onStart([]() {});
  ArduinoOTA.onEnd([]() {});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {});
  ArduinoOTA.onError([](ota_error_t error) {});
  ArduinoOTA.begin();
}
const char* IntToBool (int r) {
    if (r > 0){
      return "true";
    } else{
      return "false";
    }
}

void loop() {
  ArduinoOTA.handle();
  if (!client.connected()){
    reconnect();
  }
  client.loop();
  if(digitalRead(BTN)== LOW && (millis() - previousMillis > 2000)){
    previousMillis = millis();
    digitalWrite(REL, !digitalRead(REL));
    client.publish("myhome/Cupboard/relay", IntToBool(digitalRead(REL)));
  }
}
