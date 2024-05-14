#include <Arduino.h>
#include <Encoder.h>
#include <Codeuse.h>

// Right Encoder



#define PIN_CODEUSE_DROITE_A 26
#define PIN_CODEUSE_DROITE_B 27

#define PIN_CODEUSE_GAUCHE_A 28
#define PIN_CODEUSE_GAUCHE_B 29


Codeuse Right_Encoder, Left_Encoder;


void setup() {
  Right_Encoder = Codeuse(PIN_CODEUSE_DROITE_A, PIN_CODEUSE_DROITE_B, 16384, 0.0532);
  Left_Encoder = Codeuse(PIN_CODEUSE_GAUCHE_A, PIN_CODEUSE_GAUCHE_B, 8192, 0.0532);
  
  Serial.begin(115200);

}

void loop() {

  delay(10);
  Right_Encoder.actuate(0.010);
  Left_Encoder.actuate(0.010);


  Serial.print(Right_Encoder.debug_ticks);
  Serial.print("    ");
  Serial.print(Left_Encoder.debug_ticks);
  Serial.print("\n");



}