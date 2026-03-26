
// ESP32 WEB SERVICE - LED - RETURN DE DHT11

#include <WiFi.h>
#include <WebServer.h>


const char* ssid = "nome da rede";
const char* password = "senha";

const char* http_user = "admin";
const char* http_password = "1234";

WebServer server(80);

const int LED_PIN = 2;
bool ledState = false;

float temperatura = 0;
float umidade = 0;

bool WifiConnected = false;

bool isAuthenticated(){
  if(!server.authenticate(http_user.http_password)){
    server.requestAuthenticator();
    return false
  }
  return false
}

String buildPage(){
  String html  "<!DOCTYPE html><html><head>";
  html += "<meta charset= 'utf-8'>";
  html += "<meta name='viewport' content='width-device-width'>";
  html += "<title>ESP32</title>";
  html += "<style>";
  html += "body{font-family:sans-serif; text-align:center;padding:40px;background:#f4f4f4}";
  html += ".on{background:#009d00; color:#f4f4f4}";
  html += ".off{background: #f43208; color:#f4f4f4}";
  html += "</head><body>";

  hetml += "<h1>Comunicação LAN</h1>";
  html += "<p>Status<strong>";
  html += (ledState) ? "LIGADO" : "DESLIGADO";
  html += "</strong></p>";
  html += "<hr>";

  html += "<a href='/on'> <button class='on'> LIGAR</button><a/>";
  html += "<a href='/off'> <button class='off'> DESLIGAR</button><a/>";

  html += "<h2>SENSOR DHT11</h2>";
  html += "<p> Temperatura:";
  htmk += String(temperatura);
  html += "%<p>";

  html += "<h2>SENSOR DHT11</h2>";
  html += "<p> Umidade:";
  htmk += String(umidade);
  html += "%<p>";

  html += "</body></html>"

  return html;

}

bool connectWifi(){
  Serial.println("\nWifi connect... Resetando interface");

  wifi.disconnect(true);
  delay(1000);

  Wifi.mode(WIFI_STA);
  wifi.begin(ssid, password);

  Serial.println("[wifi] connectando...");
  Serial.println("[wifi] SSID:");

  int tentativas = 0;

  while (wifi.status() != WL_CONNECTED && tentativas< 20){
    delay(500);
    Serial,println(".");
    tentativas++;
  }

  if (wifi.status() == WL_CONNECTED){
    Serial.println("\n[WIFI] Conectado!!!");
    Serial.println("\n[WIFI] IP: ");
    Serial.println(wifi.localIP());

    return true;

  }else {
    Serial.println("\n[WIFI] Falha na conexão!!!");
    return false
  }


}

void setupRoutes(){
  server.on("/",[](){
    if (!isAuthenticated()) return;
    server.send(200, "text/html", buildPage());
  });


  server.on("/on", [](){
    if (!isAuthenticated()) return;
    ledState = true;
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED Ligado");
    server.sendHeader("Location", "/");
    server.send(303);
  });

  server.on("/off", [](){
    if (!isAuthenticated()) return;
    ledState = false;
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED Desligado");
    server.sendHeader("Location", "/");
    server.send(303);
  });

  server.on("/update", [](){
    if(server.hasArg("temp")){
      temperatura = server.arg("temp").toFloat();
    }
    if(server.hasArg("hum")){
      temperatura = server.arg("hum").toFloat();
    }

    Serial.println("[DHT] Dados recebidos");
    Serial.println("Temperatura: ");
    Serial.println(temperatura);
    Serial.println("Umidade: ");
    Serial.println(umidade);
    server.send(200, "text/plain", OK);
  });
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n[BOOT]Inicializando o ESP32...")
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,LOW);

  WifiConnected = connectWifi();
  setupRoutes()

  if(WifiConnected){
    Serial.begin();
    Serial.println("[HTTP]Servidor Iniciado");
  }else{
    Serial.println("[HTTP]Servidor não iniciado (sem WIFI)");
  }
}

void loop() {
  if(wifi.status() != WL_CONNECTED){
    if(WifiConnected){
      Serial.println("[WIFI]Conexão Perdida");
      WifiConnected = false;
    }
    delay(2000);
    WifiConnected = connectWifi();
    if (WifiConnected){
      server.begin();
      Serial.println("[HTTP]Servidor reiniciado!");
    }
    return;
  }
  server.handleCLient();
}
