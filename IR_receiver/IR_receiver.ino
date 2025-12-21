#include <Arduino.h>

#define DECODE_NEC

#include <IRremote.hpp>

#define IR_RECEIVE_PIN 4

void setup() {

  Serial.begin(115200);
  
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); 

}

void loop() {

  if (IrReceiver.decode()) {
    
    IrReceiver.printIRResultShort(&Serial);
    
    Serial.print("HEX Kodu: 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX);
    
    Serial.println("------------------------------------------------");
    IrReceiver.resume(); 
  }

}
