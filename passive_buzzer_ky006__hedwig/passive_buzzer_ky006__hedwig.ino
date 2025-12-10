
#define BUZZER_PIN 4

#define NOTE_A4  440
#define NOTE_AS4 466 // La Diyez (A Sharp)
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622 // Re Diyez / Mi Bemol
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740 // Fa Diyez
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_G6  1568
#define NOTE_B6  1976


int tempo = 85;
void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  playHedwig();
  delay(3000);
}


void playHedwig() {
  
  int melody[] = {
    
    0, 2, NOTE_D5, 4,
    NOTE_G5, -4, NOTE_AS5, 8, NOTE_A5, 4,
    NOTE_G5, 2, NOTE_D6, 4,
    NOTE_C6, -2, 
    NOTE_A5, -2,
    
    NOTE_G5, -4, NOTE_AS5, 8, NOTE_A5, 4,
    NOTE_F6, 2, NOTE_GS5, 4,
    NOTE_D6, -1, 
    NOTE_D5, 4,
    
    NOTE_G5, -4, NOTE_AS5, 8, NOTE_A5, 4,
    NOTE_G5, 2, NOTE_D6, 4,
    NOTE_F6, 2, NOTE_E6, 4,
    NOTE_DS6, 2, NOTE_B5, 4,
    
    NOTE_DS6, -4, NOTE_D6, 8, NOTE_CS6, 4,
    NOTE_CS6, 2, NOTE_B5, 4,
    NOTE_G6, -1,
    NOTE_AS5, 4,
     
    
    NOTE_D6, 2, NOTE_AS5, 4,
    NOTE_D6, 2, NOTE_AS5, 4,
    NOTE_DS6, 2, NOTE_D6, 4,
    NOTE_CS6, 2, NOTE_A5, 4,
    NOTE_AS5, -4, NOTE_D6, 8, NOTE_CS6, 4,
    NOTE_CS6, 2, NOTE_D6, 4,
    NOTE_D6, -1,
    0, 4, NOTE_AS5, 4,  
    
    NOTE_D6, 2, NOTE_AS5, 4,
    NOTE_D6, 2, NOTE_AS5, 4,
    NOTE_F6, 2, NOTE_E6, 4,
    NOTE_DS6, 2, NOTE_B5, 4,
    
    NOTE_DS6, -4, NOTE_D6, 8, NOTE_CS6, 4,
    NOTE_CS6, 2, NOTE_AS5, 4,
    NOTE_G6, -1
  };

 
  int notes = sizeof(melody) / sizeof(melody[0]) / 2;
  int wholenote = (60000 * 4) / tempo;
  int divider = 0, noteDuration = 0;

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    divider = melody[thisNote + 1];
    
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; 
    }

   
    tone(BUZZER_PIN, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(BUZZER_PIN);
  }
}



  