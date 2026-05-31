#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define TRIG_PIN D5
#define ECHO_PIN D6
#define LED_PIN D2

const char* ssid = "NomeDoWiFi";
const char* password = "SenhaDoWifi";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long ultimoEnvio = 0;
const unsigned long intervaloEnvio = 2000;

void conectarWiFi() {
  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void conectarMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT... ");
    String clientId = "grupo10-lixeira-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("conectado.");
    } else {
      Serial.print("falhou, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

float medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duracao == 0) return -1;

  float distancia = duracao * 0.034 / 2;

  if (distancia < 2 || distancia > 100) return -1;

  return distancia;
}

String classificarStatus(float distancia) {
  if (distancia < 0) return "ERRO_LEITURA";
  if (distancia < 10) return "CHEIA";
  if (distancia <= 35) return "PARCIAL";
  return "VAZIA";
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  conectarWiFi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    conectarMQTT();
  }

  client.loop();

  unsigned long agora = millis();

  if (agora - ultimoEnvio >= intervaloEnvio) {
    ultimoEnvio = agora;

    float distancia = medirDistancia();
    String status = classificarStatus(distancia);

    digitalWrite(LED_PIN, status == "CHEIA" ? HIGH : LOW);

    String payload = "{";
    payload += "\"distancia_cm\":";
    payload += String(distancia, 2);
    payload += ",";
    payload += "\"status\":\"";
    payload += status;
    payload += "\"";
    payload += "}";

    client.publish("grupo10/lixeira/nivel", payload.c_str());

    Serial.println(payload);
  }
}