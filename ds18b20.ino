#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D4   

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

const char* host = "api.thingspeak.com"; 
String path = "/update?key=S9HON8X6600I21V6&field1=";  
const int httpPort = 80;
const char* ssid = "GraphicsLab";
const char* pass = "graphics";
const char* server_ip = "163.239.78.74";

char temperatureString[6];

void setup(void){
  Serial.begin(115200);
  Serial.println("");
  
  WiFi.begin(ssid, pass);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  DS18B20.begin();
   

}

float getTemperature() {
  float temp;
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    delay(100);
  } while (temp == 85.0 || temp == (-127.0));
  return temp;
}


void loop() {

  float temperature = getTemperature();

  dtostrf(temperature, 2, 2, temperatureString);
  // send temperature to the serial console
  Serial.println(temperatureString);

  WiFiClient client;
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  client.print(String("GET ") + path + temperatureString + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");


  if (!client.connect(server_ip, httpPort)) {
    Serial.println("connection failed in server");
    return;
  }

   client.print(String("GET ") + "/?value=" + temperatureString + " HTTP/1.1\r\n" +
               "Host: " + server_ip + "\r\n" +
               "Connection: keep-alive\r\n\r\n");
  String temp = String("http://") + server_ip + "/?value=" + temperatureString +
               "&ip="+ WiFi.localIP();
  Serial.println(temp);
  delay(10000);

}
