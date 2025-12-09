#include "WiFi.h"

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true, true);
  delay(1000);  
}

void loop() {
  // put your main code here, to run repeatedly:
  // digitalWrite(2, HIGH);
  // delay(500);
  // digitalWrite(2, LOW);
  // delay(500);
  
  digitalWrite(2,HIGH);
  Serial.println("Ağlar Taranıyor...");
  int n = WiFi.scanNetworks();
  delay(500);
  digitalWrite(2,LOW);

  if (n==0){
    Serial.println("Hiç ağ bulunamadı .");
  }
  else{
    Serial.printf("%d ağ bulundu:\n",n);
    
    for (int i = 0;i < n; i++){
      Serial.printf("%d) %s  (%ddBm)  %s\n",
        i + 1,
        WiFi.SSID(i).c_str(),
        WiFi.RSSI(i),
        WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "Açık" : "Şifreli"
      );
    }
  }
  Serial.println("-----------------------");
  delay(5000);
}
