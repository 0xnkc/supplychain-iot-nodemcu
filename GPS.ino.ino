#include <TinyGPS++.h>
#include "DHT.h"
#include <SoftwareSerial.h>
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#define DHTPIN D5 
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);  

//gps
static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;

//dht sensor
uint8_t temperature, humidity;

// repace your wifi username and password
const char* ssid     = "OnePlus 5T";
const char* password = "12345678";

unsigned long myChannelNumber = 705714;
const char * myWriteAPIKey = "HP7P0QIYJRJXCRDX";

// The TinyGPS++ object
TinyGPSPlus gps;
WiFiClient  client;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  ThingSpeak.begin(client);
  
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
 
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
   
    
 // Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {

    double latitude = (gps.location.lat());
    double longitude = (gps.location.lng());
    
    String latbuf;
    latbuf += (String(latitude, 6));
    Serial.println(latbuf);

    String lonbuf;
    lonbuf += (String(longitude, 6));
    Serial.println(lonbuf);

    ThingSpeak.setField(1, latbuf);
    ThingSpeak.setField(2, lonbuf);
    ThingSpeak.setField(3,temperature);
    ThingSpeak.setField(4,humidity);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  
    delay(1000);
    
  }
  else
  {
    Serial.print(F("INVALID"));
  }


  Serial.println();
}
