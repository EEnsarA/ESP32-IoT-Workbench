const int ledPin = 23;
const int sensorPin = 34;
const int buzzerPin = 4;


const int HASSASIYET = 20;
const int BEKLEME_SURESI = 300; 
float esikDeger = 0;
unsigned long sonVurusZamani = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);
  
  esikDeger = analogRead(sensorPin);
}

void loop() {

  int anlikDeger = analogRead(sensorPin);
  unsigned long simdikiZaman = millis();

  if (anlikDeger > esikDeger + HASSASIYET) {
    
    if(simdikiZaman - sonVurusZamani > BEKLEME_SURESI){

      digitalWrite(ledPin, HIGH); 
      Serial.println("Nabız Algılandı!"); 
      tone(buzzerPin, 600, 80);

      sonVurusZamani = simdikiZaman;                 
    }

  } else {
    noTone(buzzerPin);
    digitalWrite(ledPin, LOW);  // LED'i Söndür
  }

  esikDeger = (esikDeger * 0.96) + (anlikDeger * 0.04);
  Serial.print(esikDeger + HASSASIYET); 
  Serial.print(" ");
  Serial.print(esikDeger);            
  Serial.print(" ");
  Serial.println(anlikDeger);

  delay(20);
  // digitalWrite(ledPin, HIGH);  // (3.3V ver)
  // Serial.println("LED Yandı!");
  // delay(1000);

  // digitalWrite(ledPin, LOW);   // (0V ver)
  // Serial.println("LED Söndü!");
  // delay(1000);
}
