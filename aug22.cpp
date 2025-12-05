


#include <DHT.h>
#include <DHT_U.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

//------DISP


//-------DHT
#define DHTPIN D5     // наш датчик подключен на этот контакт
#define DHTTYPE DHT11   // тип датчика DHT 11
DHT dht(DHTPIN, DHTTYPE); // инициализируем д

//-----temp sensor


const char* ssid = "silicon"; // WiFi name
const char* password =  "xid123mt"; //WiFi password
const char* mqttServer = "m2.wqtt.ru";
const int mqttPort = 14865;
const char* mqttUser = "u_JVM2NJ";
const char* mqttPassword = "FZTZUrDg";
uint8_t* tempMes;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  Serial.begin(115200);
  dht.begin();
  //sensors.begin();
  WiFi.begin(ssid, password);
  

  
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.println("Connecting to WiFi..");
   
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
  

  Serial.print("Connecting to MQTT...");

    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      
        Serial.println(" Connected");  
        //myOLED.print("Connected", 1, 51);
  delay(3000);
   
    } else {
      
      Serial.print(" Failed with state ");
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
 char result[4];
    dtostrf(temp, 2, 2, result);
	client.publish("/temp",result,true);
}

void sendTime(int min){
 char result[4];
    dtostrf(min, 2, 2, result);
	client.publish("/minutes",result,true);
}

void sendTimeSec(uint32_t sec){
 char result[4];
    dtostrf(sec, 2, 2, result);
	client.publish("/sec",result,true);
}

void sendOutTemp(float temp){
	Serial.print("New OutTemperature: ");
	Serial.println(temp);
  
 char result[4];
    dtostrf(temp, 2, 2, result);
	client.publish("/outtemp",result,true);
}

void sendHumi(float humi){
	//Serial.print("New Humidity: ");
	//Serial.println(humi);
  char result[4];
    dtostrf(humi, 2, 2, result);
	client.publish("/humi",result,true);
}

void loop() {

  delay(1000);
  

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  sendTemp(t);
  sendHumi(h);
  Serial.print("Влажность: ");
  Serial.print(h);
  Serial.print(" % ");
  Serial.print("Tемпература: ");
  Serial.print(t);
  Serial.println(" *C ");

  uint32_t sec = millis() / 1000ul;      // полное количество секунд
  int allMin = sec / 60;
  int timeHours = (sec / 3600ul);        // часы
  int timeMins = (sec % 3600ul) / 60ul;  // минуты
  int timeSecs = (sec % 3600ul) % 60ul;  // секунды

  Serial.print("Hours: ");
  Serial.print(timeHours);
  Serial.print(" Minutes: ");
  Serial.print(timeMins);
  Serial.print(" Seconds: ");
  Serial.println(timeSecs);
  sendTime(allMin);
  sendTimeSec(sec);

}
