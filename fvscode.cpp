#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 5     // наш датчик подключен на этот контакт
#define DHTTYPE DHT11   // тип датчика DHT 11
DHT dht(DHTPIN, DHTTYPE); // инициализируем д


const char* ssid = "silicon"; // WiFi name
const char* password =  "xid123mt"; //WiFi password
const char* mqttServer = "m2.wqtt.ru";
const int mqttPort = 14866;
const char* mqttUser = "u_JVM2NJ";
const char* mqttPassword = "FZTZUrDg";
uint8_t* tempMes;


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  Serial.begin(115200);
  dht.begin();
  //----------
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
  
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
        Serial.println("connected");  
        
  delay(3000);
    
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe("/temp");

}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void sendTemp(float temp){
	Serial.print("New temperature: ");
	Serial.println(temp);
  
 char result[4];
    dtostrf(temp, 2, 2, result);
	client.publish("/temp",result,true);
}

void sendOutTemp(float temp){
	Serial.print("New OutTemperature: ");
	Serial.println(temp);
  
 char result[4];
    dtostrf(temp, 2, 2, result);
	client.publish("/outtemp",result,true);
}

void sendHumi(float humi){
	Serial.print("New Humidity: ");
	Serial.println(humi);
  char result[4];
    dtostrf(humi, 2, 2, result);
	client.publish("/humi",result,true);
}
/* insidePrint
void insidePrint(String t, String h){
  myOLED.clrScr();
  myOLED.print("Дома", OLED_C, 15);
  myOLED.print("Темп", 1, 43);
  myOLED.print(t, 55, 43);
  myOLED.print("Влаж", 1, 61);
  myOLED.print(h, 55, 61);
}

*/
/*
void outsidePrint(String t, String h){
  myOLED.clrScr();
  myOLED.print("На улице", OLED_C, 15);
  myOLED.print("Темп", 1, 43);
  myOLED.print(t, 55, 43);
  myOLED.print("Влаж", 1, 61);
  myOLED.print(h, 55, 61);
}
*/



void loop() {
  //delay(5000);
  //sensors.requestTemperatures(); 
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
 
  if (isnan(h) || isnan(t) ) {
    Serial.println("Ошибка считывания из DHT датчика!");
    
    return;
  }
  //insidePrint(String(t),String(h));
  delay(5000);
  //outsidePrint(String(outT),"0");
  delay(5000);
 
  sendTemp(t);
  sendHumi(h);
  //sendOutTemp(outT);

  

}


/* OLD CODE!!!!!!!!!
const char *ssid = "silicon"; // Enter your WiFi name
const char *password = "xid123mt";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "emqx/esp32";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;
// put function declarations here:
WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void setup() {
  
  // put your setup code here, to run once:
 // Set software serial baud to 115200;
    Serial.begin(115200);
    // Connecting to a WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to the Wi-Fi network");
    //connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public EMQX MQTT broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    // Publish and subscribe
    client.publish(topic, "Hi, I'm ESP32 ^^");
    client.subscribe(topic);
}



void loop() {
  // put your main code here, to run repeatedly:
  client.loop();
}

*/
// WiFi
