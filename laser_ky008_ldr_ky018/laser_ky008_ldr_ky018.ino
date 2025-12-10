#define LDR_PIN 34
#define LASER_PIN 2
#define BUZZER_PIN 4
#define LED_PIN 23

#define THRESHOLD_VALUE 550


void setup() {  
  // put your setup code here, to run once:
  pinMode(LASER_PIN,OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  pinMode(LDR_PIN,INPUT);

  Serial.begin(115200);
  Serial.printf("LDR Threshold Value : %d\n",THRESHOLD_VALUE);
  Serial.println("Lazer açık hedef bekleniyor ...");
  digitalWrite(LASER_PIN,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  int ldrValue = analogRead(LDR_PIN);
  Serial.printf("LDR Value : %d\n",ldrValue);
  if(ldrValue < THRESHOLD_VALUE)
  {
    tone(BUZZER_PIN, 1000);
    digitalWrite(LED_PIN,HIGH);
    Serial.println("Hedef Bulundu !");
  }else{
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN,LOW);
    digitalWrite(LED_PIN,LOW);
  }
  delay(100);
}
