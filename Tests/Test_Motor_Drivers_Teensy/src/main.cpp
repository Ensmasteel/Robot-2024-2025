#include <Arduino.h>

//Right Motor
#define PIN_RIGHT_MOTOR_PWM 33
#define PIN_RIGHT_MOTOR_IN1 34
#define PIN_RIGHT_MOTOR_IN2 35

//Left Motor
#define PIN_LEFT_MOTOR_PWM 36
#define PIN_LEFT_MOTOR_IN1 37
#define PIN_LEFT_MOTOR_IN2 38

int count =0;

void setup() {

  Serial.begin(115200);

  pinMode(PIN_RIGHT_MOTOR_IN1, OUTPUT);
  pinMode(PIN_RIGHT_MOTOR_IN2, OUTPUT);
  pinMode(PIN_RIGHT_MOTOR_PWM, OUTPUT);
  pinMode(PIN_LEFT_MOTOR_IN1, OUTPUT);
  pinMode(PIN_LEFT_MOTOR_IN2, OUTPUT);
  pinMode(PIN_LEFT_MOTOR_PWM, OUTPUT);

  //analogWriteResolution(bitResolution); //needs to be used to increase Write reolution
  analogWriteResolution(12);
  analogWriteFrequency(PIN_RIGHT_MOTOR_PWM, 29296.875);
  analogWriteFrequency(PIN_LEFT_MOTOR_PWM, 29296.875);
  
  //Set rotation direction (refer to motor driver datasheet)
  digitalWrite(PIN_RIGHT_MOTOR_IN1, HIGH);
  digitalWrite(PIN_RIGHT_MOTOR_IN2, LOW);
  digitalWrite(PIN_LEFT_MOTOR_IN1, LOW);
  digitalWrite(PIN_LEFT_MOTOR_IN2, HIGH);
}

void loop() {
  count=max(count+10,2050);
  delay(10);
  analogWrite(PIN_RIGHT_MOTOR_PWM, count); //moteur droit
  analogWrite(PIN_LEFT_MOTOR_PWM, count); //left
    
  //Right Motor
  /*
  for(int i = 0; i<255; i++){
    analogWrite(PIN_RIGHT_MOTOR_PWM, i);
    Serial.println(i);
    delay(100);
  }
  for(int i = 0; i<255; i++){
    analogWrite(PIN_RIGHT_MOTOR_PWM, 255-i);
    Serial.println(255-i);
    delay(100);
  }
 
  //Left Motor
  for(int i = 0; i<255; i++){  
    analogWrite(PIN_LEFT_MOTOR_PWM, i);
    analogWrite(PIN_RIGHT_MOTOR_PWM, i);
    Serial.println(i);
    delay(100);
  }
  for(int i = 0; i<255; i++){
    analogWrite(PIN_LEFT_MOTOR_PWM, 255-i);
    analogWrite(PIN_RIGHT_MOTOR_PWM, 255-i);
    Serial.println(255-i);
    delay(100);
  }
  */


}