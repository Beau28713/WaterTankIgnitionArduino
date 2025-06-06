#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoModbus.h>

#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

const int relay_pin = LED_BUILTIN;
const int potentiometer_pin = A0;
int status = WL_IDLE_STATUS;

WiFiServer server(502);
ModbusTCPServer modbusTCPServer;

void updaterelay() {
  bool state = modbusTCPServer.coilRead(0x00);
  digitalWrite(relay_pin, state ? HIGH : LOW);
}

void updatepotentiometer() {
  int potValue = analogRead(potentiometer_pin);
  float potVoltage = potValue *(5.0/1024.0);

  uint32_t dualValue1 = *((uint32_t *)&potVoltage); // Convert float to uint32_t for Modbus
  uint16_t register1 = dualValue1 >> 16;  // right shift >> by 16bits
  uint16_t register2 = dualValue1 & 0xffff; // mask to get the lower 16 bits

  int potResistence = map(potValue, 0, 1023, 0, 10000);
  delay(10); // Allow for analog read settling
  modbusTCPServer.inputRegisterWrite(0x02, potResistence); // Write potentiometer value to input register
  modbusTCPServer.inputRegisterWrite(0x00, register1); // Write potentiometer voltage to input register
  modbusTCPServer.inputRegisterWrite(0x01, register2); // Write potentiometer voltage to input register
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  // Connect to WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Connecting to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }

  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start TCP server
  server.begin();

  // Start ModbusTCPServer and configure coils
  if (!modbusTCPServer.begin()) {
    Serial.println("Modbus TCP Server failed to start!");
    while (1);
  }

  modbusTCPServer.configureCoils(0x00, 5);  // Configure 5 coils starting at address 0x00 Modbus address default starts at 1 in ignition. 
                                            // you must go into advance setting in ignition to set the starting address to 0 by checkng the box in device connections in your device you are using.

  modbusTCPServer.configureInputRegisters(0x00, 5); // Configure 5 input registers starting at address 0x00

  
  pinMode(relay_pin, OUTPUT);
  pinMode(potentiometer_pin, INPUT);

  // Initialize relay to LOW (off)
  digitalWrite(relay_pin, LOW);

  Serial.println("Modbus server started.");
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client connected");
    modbusTCPServer.accept(client);  // Important: bind the client!

    while (client.connected()) {
      modbusTCPServer.poll();        // Handle Modbus request
      updaterelay();                 // Sync output to coil state
      updatepotentiometer();         // Sync input register to potentiometer value
      delay(10);                     // Allow brief socket breathing room
    }

    client.stop();
    Serial.println("Client disconnected");
  }
}
