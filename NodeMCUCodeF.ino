#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "ThingSpeak.h"

#define PIR D5

static const int RXPin = D8, TXPin = D7;
static const uint32_t GPSBaud = 9600;
SoftwareSerial ss(RXPin, TXPin);
const char ssid[] = "DEVICE1234";  
const char pass[] = "CARE1234";
unsigned long myChannelNumber = 1921180;         
const char * myWriteAPIKey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

String latbuf,lonbuf;
float latitude,longitude;
int pirval;

TinyGPSPlus gps;

WiFiClient client;

void setup()
{
Serial.begin(115200);
ss.begin(GPSBaud);
pinMode(PIR,INPUT);
ThingSpeak.begin(client);
WiFi.begin(ssid, pass);
}

void loop()
{
/*GPS TRACKING*/
while (ss.available() > 0)
if (gps.encode(ss.read()))
displayInfo();
if (millis() > 5000 && gps.charsProcessed() < 10)
{
Serial.println(F("No GPS detected: check wiring."));
while(true);
} 
/*PIR SENSOR*/
pirval = digitalRead(PIR);
if (pirval == 1)
{
Serial.print("Human Detected at ");
Serial.print("Latitude : ");
Serial.print(latbuf);
Serial.print(" Longitude : ");
Serial.println(lonbuf); 
}
else
{
Serial.println("Human Not Detected");  
}
/*THINGSPEAK*/
ThingSpeak.setField(1, pirval);
ThingSpeak.setField(2, latitude);
ThingSpeak.setField(3, longitude);
ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
}

void displayInfo()
{
if (gps.location.isValid())
{
latitude = (gps.location.lat());
longitude = (gps.location.lng());
latbuf = (String(latitude, 6));
lonbuf = (String(longitude, 6));
}
else
{
latitude = 19.017449; //add latitude
longitude = 72.819711; //add longitude
latbuf = "19.017449"; //add latitude
lonbuf = "72.819711"; //add longitude
}
}
