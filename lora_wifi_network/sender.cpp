#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>

//int counter = 0;
//int moisture_percent = 0;


void setup() {
  Serial.begin(115200);
  SPI.begin(5,19,27,18);
  LoRa.setSPI(SPI);
  LoRa.setPins(18, 14, 26);
  Serial.println("Hello World");
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  int val;
  Serial.print("Sending packet: ");
  //Serial.println(counter);
  val = analogRead(A0); //connect sensor to Analog 0
  Serial.println(val); //print the value to serial port
  // send packet
  LoRa.beginPacket();
  LoRa.print("Moisture value: ");
  LoRa.print(val);
  LoRa.endPacket();

  //counter++;

  delay(5000);
}