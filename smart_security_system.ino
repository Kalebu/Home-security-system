#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include "SR04.h"
#define TRIG_PIN A1
#define ECHO_PIN A0
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
int distance;
int distance_prev=true;

SoftwareSerial GSM_Module(3, 2);
LiquidCrystal_I2C lcd(0x3f,16,2);

int red_led = 10;
int blue_led = 13;
int buzzer = 11;
int panic_button = A3;
int door_pressed = A2;

bool panic_prev = true;
int panic_detected;

bool door_prev = true;
int door_signal;

void welcoming(){

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Alarm System");
      lcd.setCursor(5, 1);
      lcd.print("Online");
}



void setup() {
    Serial.begin(9600);
    GSM_Module.begin(9600);
    send_sms("Alarm System \n Online");
    pinMode(blue_led, OUTPUT);
    pinMode(red_led, OUTPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(panic_button, INPUT_PULLUP);
    pinMode(door_pressed, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();
    welcoming();
}

void loop() {

        panic_detected = digitalRead(panic_button);
        door_signal = digitalRead(door_pressed);
        distance = sr04.Distance();;
        Serial.println(distance);
        if (panic_detected == 0 | door_signal == 0){
          if (panic_detected){
            if (panic_prev){
                arm_the_system("Emergency pressed");
                panic_prev = false;
            }
            else panic_prev = true;
          }
         else if (door_signal==1){
            if (door_prev){
              arm_the_system("Door opened");
              door_prev = false;
            }
            else door_prev = true;
         }
        }
      else{
        door_prev = true;
        panic_prev=true;
      }

      if (distance<=20){
        if (distance_prev){
              arm_the_system("Motion Detected"); 
              distance_prev=false;
        }
      }
      else distance_prev = true;
delay(100);
}

void arm_the_system(String notification){
    lcd.clear();
    lcd.setCursor(0, 0);
    Serial.println(notification);
    lcd.print(notification);
    send_sms(notification);
    digitalWrite(red_led, 1);
    digitalWrite(buzzer, 1);
    delay(5000);
    digitalWrite(red_led, 0);
    digitalWrite(buzzer, 0);
    welcoming();
}




void send_sms(String Message){
    GSM_Module.println();
    GSM_Module.println("AT");
    delay(100);
    GSM_Module.println("AT+CMGF=1");
    delay(100);
    GSM_Module.println("AT+CMGS=\"+255782536312\"\r"); 
    delay(1000);
    GSM_Module.println(Message);
    delay(100);
    GSM_Module.println((char)26);
    Serial.print(Message);Serial.print(" sent");
}
