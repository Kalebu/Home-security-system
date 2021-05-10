#include<SoftwareSerial.h>

SoftwareSerial GSM_Module(3, 2);

void setup() {
  Serial.begin(9600);
  GSM_Module.begin(9600);
}
void loop() {
      walk();
}

void walk(){

    while(GSM_Module.available()){
        Serial.write(GSM_Module.read());
    }

    while (Serial.available()){
        GSM_Module.write(Serial.read());
    }
  
}
