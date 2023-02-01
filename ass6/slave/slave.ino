#include <Wire.h>
#define SLAVE_ADDR 9
int LED = 13;
int rd;
int br;

void setup() {
  pinMode(LED, OUTPUT);
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void receiveEvent() {
  rd = Wire.read();
  Serial.println(rd);
}

void loop (){
  delay(50);
  br = map(rd, 1,255,100,2000);
  digitalWrite(LED,HIGH);
  delay(br);
  digitalWrite(LED,LOW);
  delay(br);
}
