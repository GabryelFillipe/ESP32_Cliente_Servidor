
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "nome da rede";
const char* password = "senha";

const char*serverIP = "192.168.1.0"; //Alterar quando o ESP retornar

unsigned long lastSend = 0;
const long interval = 5000;

bool wifiConnected = false;

bool connectWifi(){
  Serial.println("\n[WIFI]Resetando a intercafe...")
  WiFi.disconnected(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
    Serial.println("\n[WIFI]Conectando...")
    Serial.println("\n[WIFI]ssid: ")
    Serial.println(ssid)

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas< 20){
    delay(500);
    Serial,println(".");
    tentativas++;
  }

  if (WiFI.status() == WL_CONNECTED){
    Serial.println("\n[WIFI] Conectado!!!");
    Serial.println("\n[WIFI] IP: ");
    Serial.println(WiFI.localIP());

    return true;

  }else {
    Serial.println("\n[WIFI] Falha na conexão!!!");
    return false
  }
}

void sendData(float temp, float hum){

  HTTPClient http;

  String url = "http://"+String(serverIP) + "/update?temp="+ String(temp,1)+"&hum=" + String(hum,1);
  Serial.println("[HTTP]Enviando: ");
  Serial.println(url);
  http.begin(url);
  http.setTimeOut(3000);

  int httpCode = http.GET();
  if (httpCode > 0){
    Serial.println("[HTTP]Resposta: ");
    Serial.println(payload);
  } else {
    Serial.println("[HTTP]ERROR: ");
    Serial.println(httpCode)
  }

  http.end();

}



void setup() {

}

void loop() {

}
