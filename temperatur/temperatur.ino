/*

#####################################
# Janek's Temperatur #
#####################################

  Hardware:
  ---------
  - Arduino Diecimila mit ATMEGA168
  - LM35 Temperatur-Sensor (an Analog 0)

  Funktionen:
  -----------
  
  Versionsinfo:
  -------------
  
*/

#include <Ethernet.h>
#include <SPI.h>

// Arduino Ethernet Information
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x63, 0xA8 };
byte ip[]  = { 192, 168, 0, 177 };
byte gateway[] = { 192, 168, 0, 1 };
byte subnet[]  = { 255, 255, 255, 0 };

// Server Information
byte server[] = { 46, 252, 18, 45 };
//byte server[] = { 192, 168, 0, 1 };
Client client(server, 80);

char host[] = "arduino.nicetrash.de";
char url[]  = "/temperature.php";
char key[] = "12345";
char c;

// Pin Configuration
int sensorPin = 0;                      // Temperature Sensor Pin
int lightPin = 1;                       // Light Sensor Pin
int ledPin = 13;         		// Power LED Pin

// Variables
float Temperatur = 0;      		// Variable für Temperatur ;-)
int temp = 0;            		// Analog-Wert
int light = 0;

void setup()
{
  pinMode(ledPin, OUTPUT); 
  
  Serial.begin(9600);			// für Debug-Ausgaben
  Serial.println("booting system...");
  Serial.println();
  digitalWrite(ledPin, HIGH);
  
  Ethernet.begin(mac, ip);  
  delay(5000);				// warten, bis LAN gestartet
  Serial.println("system ready");
  Serial.println();
  digitalWrite(ledPin, LOW);
  
  if (client.connect()) {
    Serial.println("connected");
    client.println("GET / HTTP/1.1");
    client.println("Host: nicetrash.de");
    client.println();
  } else {
    Serial.println("connection failed");
  }
}

void loop()
{
  
  digitalWrite(ledPin, HIGH);

  readSensor();
  client.stop();
  client.flush();
  
  // Daten schreiben
  sendData();
  
  digitalWrite(ledPin, LOW);
  
  delay(300000);
}

void readSensor()
{
  temp = analogRead(sensorPin);               // Analog-Wert auslesen
  light = analogRead(lightPin);               // Analog-Wert auslesen
  Temperatur = (0.4 * temp * 100.0)/1024.0;   // Berechnung der Temperatur in °C
  Serial.print("Temperatur: ");
  Serial.println(Temperatur);
  Serial.print("Licht: ");
  Serial.println(light);
}

void sendData()
{
  if(client.connect())  // Verbindung zum Server aufbauen
  {
    Serial.println("connected");
    
    client.print("GET ");
    client.print(url);
    client.print("?temp=");
    client.print(Temperatur);
    client.print("&light=");
    client.print(light);
    client.print("&key=");
    client.print(key);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(host);
    client.println();
    Serial.println("fertig!");
  }
  else  {
    Serial.println("connection failed");
  }    
}
