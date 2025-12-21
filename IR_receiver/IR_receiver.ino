#include <Arduino.h>

#define DECODE_NEC

#include <IRremote.hpp>

#define IR_RECEIVE_PIN 4

void setup() {

  Serial.begin(115200);
  
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); 

  Serial.println("Sistem Hazır! Kumandayı sensöre tut ve tuşlara bas...");

}

void loop() {

  if (IrReceiver.decode()) {
    
    // Özet bilgiyi ekrana yaz
    // Burada "Protocol=NEC Address=0x..." gibi şeyler yazar.
    IrReceiver.printIRResultShort(&Serial);
    
    // Asıl işimize yarayan HEX kodunu ayrıca yazdıralım
    Serial.print("HEX Kodu: 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX);
    
    Serial.println("------------------------------------------------");

    // Dinlemeye devam et (Bunu demezsen tek tuştan sonra donar)
    IrReceiver.resume(); 
  }

}
