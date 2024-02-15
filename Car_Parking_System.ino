#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <String.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo myservo;          //servo motor

#define intrare1 4      //primul pin de la intrare
#define intrare2 2      //al doilea pin de la intrare

#define loc1 5          //pinul pentru primul loc de parcare
#define loc2 6          //pinul pentru al doilea loc de parcare
#define loc3 7          //pinul pentru al treilea loc de parcare
#define loc4 8          //pinul pentru al patrulea loc de parcare

#define led_rosu 13     //pinul corespunzator ledului rosu  - se aprinde cand parcarea este full si o masina incearca sa intre
#define led_alb 12      //pinul corespunzator ledului alb   - se aprinde in momentul in care se deschide bariera
#define led_albastru 11 //pinul corespunzator ledului albastru - se aprinde cand in parcare sunt locuri disponibile

int S1 = 0, S2 = 0, S3 = 0, S4 = 0;    //semnale citite de pe pinii senzorilor pentru fiecare loc de parcare
int flag1 = 0, flag2 = 0; 

int locuri = 4;         //nr de locuri disponibile

char libere [8] = "", ocupate[8] = "";  //pentru afisarea pe lcd
int contor_libere = 0;
int contor_ocupate = 0;

void setup() {  
  
  pinMode(intrare1, INPUT); //setam pinii de la senzorii aflati la intrare ca si input
  pinMode(intrare2, INPUT);
  
  pinMode(loc1, INPUT);     //setam pinii de la senzorii aflati in dreptul fiecarui loc de parcare ca si input
  pinMode(loc2, INPUT);
  pinMode(loc3, INPUT);
  pinMode(loc4, INPUT);

  
  pinMode(led_rosu, OUTPUT);  //setam pinii de la leduri ca si output
  pinMode(led_alb, OUTPUT);
  pinMode(led_albastru, OUTPUT);
 
  myservo.attach(9);          //punem servo motor ul pe pinul 9
  myservo.write(90);

  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print(" Buna ziua! ");
  
  delay(2000);
  lcd.clear();
  
  Read_Sensor();
  
  int total = S1 + S2 + S3 + S4;
  locuri = locuri - total;
}
void loop() {
  
  Read_Sensor();
  
  lcd.setCursor(0, 0);
  lcd.print("FREE: ");
  lcd.print(locuri);
  lcd.print("  ");
  
  lcd.setCursor(7, 0);
  lcd.print("| BUSY: ");
  lcd.print(4 - locuri);
  lcd.print("  ");

  strcpy(libere, "");
  strcpy(ocupate,"");
  
  contor_libere = 0;
  contor_ocupate = 0;
  
  if (S1 == 1) {
    ocupate[contor_ocupate ++] = '1';
    ocupate[contor_ocupate ++] = ' ';
    libere[contor_libere ++] = ' ';
    libere[contor_libere ++] = ' ';
  } else {
    libere[contor_libere ++] = '1';
    libere[contor_libere ++] = ' ';
    ocupate[contor_ocupate ++] = ' ';
    ocupate[contor_ocupate ++] = ' ';
  }

  if (S2 == 1) {
    ocupate[contor_ocupate ++] = '2';
    ocupate[contor_ocupate ++] = ' ';
    libere[contor_libere ++] = ' ';
    libere[contor_libere ++] = ' ';
  } else {
    libere[contor_libere ++] = '2';
    libere[contor_libere ++] = ' ';
    ocupate[contor_ocupate ++] = ' ';
    ocupate[contor_ocupate ++] = ' ';
  }
  if (S3 == 1) {
    ocupate[contor_ocupate ++] = '3';
    ocupate[contor_ocupate ++] = ' ';
    libere[contor_libere ++] = ' ';
    libere[contor_libere ++] = ' ';
  } else {
    libere[contor_libere ++] = '3';
    libere[contor_libere ++] = ' ';
    ocupate[contor_ocupate ++] = ' ';
    ocupate[contor_ocupate ++] = ' ';
  }
 
  if (S4 == 1) {
    ocupate[contor_ocupate ++] = '4';
    ocupate[contor_ocupate ++] = ' ';
    libere[contor_libere ++] = ' ';
    libere[contor_libere ++] = ' ';
  } else {
    libere[contor_libere ++] = '4';
    libere[contor_libere ++] = ' ';
    ocupate[contor_ocupate ++] = ' ';
    ocupate[contor_ocupate ++] = ' ';
  }
  
  lcd.setCursor(0, 1);
  lcd.print(libere);
  
  lcd.setCursor(7,1);
  lcd.print("| ");
  lcd.print(ocupate);
  
  digitalWrite(led_alb,LOW);
  digitalWrite(led_rosu, LOW);
  
  if(locuri == 0){
    digitalWrite(led_albastru, LOW);
  }else{
    digitalWrite(led_albastru, HIGH);
  }
  if (digitalRead(intrare1) == 0 && flag1 == 0) {
    if (locuri > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        myservo.write(180);
        locuri = locuri - 1;
        digitalWrite(led_alb, HIGH);
        delay(500);
      }
    } else {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Parking Full :(");
      digitalWrite(led_rosu,HIGH);
      delay(1500);
    }
  }
 
  if (digitalRead(intrare2) == 0 && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0 && locuri < 4) {
      myservo.write(180);
      locuri = locuri + 1;
      digitalWrite(led_alb, HIGH);
      delay(500);
    }
    digitalWrite(led_albastru, HIGH);
  }
 
  if (flag1 == 1 && flag2 == 1) {
    delay(1000);
    myservo.write(90);
    flag1 = 0, flag2 = 0;
    digitalWrite(led_albastru, HIGH);
  }
  
  delay(1);
}
void Read_Sensor() {
  S1 = 0, S2 = 0, S3 = 0, S4 = 0;
 
  if (digitalRead(loc1) == 0) {
    S1 = 1;
  }
  if (digitalRead(loc2) == 0) {
    S2 = 1;
  }
  if (digitalRead(loc3) == 0) {
    S3 = 1;
  }
  if (digitalRead(loc4) == 0) {
    S4 = 1;
  }
}
