/*
  WiFi Modbus TCP Server

  Circuit:
   - Uno WIFI Rev 2

  created 6 Oct 2024
  by Beau Clark
*/

#include <SPI.h>
#include <WiFiNINA.h>

#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

#include "arduino_secrets.h"

// this line added  for testing git updating

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

const int relay_pin = LED_BUILTIN;

int status = WL_IDLE_STATUS;

WiFiServer wifiServer(502);

ModbusTCPServer modbusTCPServer;

void updaterelay()
{
  int coilValue = modbusTCPServer.coilRead(0x00);

  if (coilValue)
  {
    digitalWrite(relay_pin, HIGH);
  }
  else
  {
    digitalWrite(relay_pin, LOW);
  }
}

void printWifiStatus()
{
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void update_water_sensor()
{
  
}

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect
  }

  Serial.println("Modbus TCP Server LED");

  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  printWifiStatus();

  wifiServer.begin();

  if (!modbusTCPServer.begin())
  {
    Serial.println("Failed to start Modbus Server!");
    while (1)
      ;
  }

  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);

  modbusTCPServer.configureCoils(0x00, 1);
}

void loop()
{
  WiFiClient client = wifiServer.available();

  if (client)
  {
    Serial.println("new client");

    modbusTCPServer.accept(client);

    while (client.connected())
    {
      modbusTCPServer.poll();

      updaterelay();
      update_water_sensor();
    }

    Serial.println("client disconnected");
  }
}
