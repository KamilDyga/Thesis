#include "SPI.h"
#include "MFRC522.h"
#include <LiquidCrystal.h>

#define SS_PIN 53
#define RST_PIN 49
#define green 8 
#define red 9
#define yellow 10

MFRC522::MIFARE_Key key;
MFRC522 rfid(SS_PIN, RST_PIN);

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

int i;
unsigned long tstart=0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, INPUT_PULLUP);
}

void loop() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Przyloz karte");
  
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Zmień kartę magnetyczną"));
    return;
  }
  String strID = "";
  for (byte i = 0; i < 4; i++) {
                                strID +=
                                (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
                                String(rfid.uid.uidByte[i], HEX) +
                                (i != 3 ? ":" : "");
  }
  strID.toUpperCase();
  Serial.print("Numer karty: ");
  Serial.println(strID);
  delay(1000);

  if (strID.indexOf("2E:33:0E:71") >= 0)
  {
    Serial.println("Ładowanie w trakcie, aby zakończyć ładowanie wciśnij przycisk");
    tstart=millis();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Witaj Kamil");
    delay (3000);
    lcd.clear();
    lcd.print("Kliknij przycisk");
    lcd.setCursor(0,1);
    lcd.print("aby zakonczyc");
    digitalWrite(8, HIGH);
    delay (2000);
    digitalWrite(8, LOW);
    
    do{
       digitalWrite(10, HIGH);
       delay (200);
       digitalWrite(10, LOW);
       delay (200);
       }
    while (digitalRead(11)==HIGH);{ 
                                    Serial.println("Koniec ładowania");
                                    lcd.clear();
                                    lcd.setCursor(0,0);
                                    lcd.print((millis()-tstart)/1000);
                                    lcd.setCursor(0,1);
                                    lcd.print("czas postoju [s]");
                                    Serial.println("Ładowanie trwało: ");
                                    Serial.println((millis()-tstart)/1000);
                                    Serial.println("Przyłóż kartę");
                                    delay(3000);
                                    return;    
                                  }
  }
        
 
  else {
        Serial.println("Dostęp niedozwolony");
        digitalWrite(9, HIGH);
        digitalWrite(9, LOW);
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Brak dostepu");
        delay(2000);
        Serial.println("Przyłóż kartę");
        return;
        } 
  

}
