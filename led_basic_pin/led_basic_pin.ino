const int ledPin = 23;
const int sensorPin = 34;

const int HASSASIYET = 7;
float esikDeger = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

  esikDeger = analogRead(sensorPin);
}

void loop() {

  int anlikDeger = analogRead(sensorPin);
  Serial.printf("Anlık değer : %d \n",anlikDeger);

  if (anlikDeger > esikDeger + HASSASIYET) {
    digitalWrite(ledPin, HIGH); 
    Serial.println(" IB IB! (Nabız Algılandı)"); 
  } else {
    digitalWrite(ledPin, LOW);  // LED'i Söndür
  }

  esikDeger = (esikDeger * 0.95) + (anlikDeger * 0.05);
  Serial.printf("Yeni esik değer : %.2f \n",esikDeger);
  delay(20);
  // digitalWrite(ledPin, HIGH);  // (3.3V ver)
  // Serial.println("LED Yandı!");
  // delay(1000);

  // digitalWrite(ledPin, LOW);   // (0V ver)
  // Serial.println("LED Söndü!");
  // delay(1000);
}
