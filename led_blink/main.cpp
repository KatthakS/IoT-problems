#include <Arduino.h>
#include <U8x8lib.h>    /* Library to interface with OLED screen  */

#define LED 25  /* Default pin for LED on Heltec board  */

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     /* Serial baud rate at 115200, default is 9600 */
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  Serial.println("Hi there!");
  u8x8.println("Hello World!");
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hi there in the loop!");
  u8x8.println("Hello World!");
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
}