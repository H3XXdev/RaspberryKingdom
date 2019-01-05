//########################################
//#                                      #
//#          RFID Lock Door v.1          #  
//#            by Jakub Szuba            #
//#                                      #
//########################################

#include "SPI.h"
#include "MFRC522.h"
#include <Servo.h> 

#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN A0 
#define LED_PIN A1

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

int servoPin = 8; 
Servo Servo2;
void setup() {
  // Kod startowy:
  Servo2.attach(servoPin);
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  Serial.println("Zbliż kartę do czytnika...");
}

void loop() {
  // Główny kod powtarzający:
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Twoja karta nie jest typu: MIFARE Classic."));
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
  Serial.print("Twój numer karty: ");
  Serial.println(strID);
  delay(1000);

  if (strID.indexOf("90:1E:57:7A") >= 0) {  //Twoja karta RFID;
    Serial.println("********************");
    Serial.println("**Dostęp przyznany**");
    Serial.println("********************"); 
    tone(A2, 1000, 400); // 4000Hz na pinie A2
    delay (400);
    tone(A2, 1500, 400); // 4000Hz na pinie A2
           Servo2.write(180);digitalWrite(A0, HIGH);
           delay (1000);
           Servo2.write(0); 
    digitalWrite(A0, HIGH);
    delay (3000);
    digitalWrite(A0, LOW);
    noTone(A2);  
    return;
  }
  else {
    Serial.println("**********************");
    Serial.println("**Dostęp zablokowany**");
    Serial.println("**********************");
    tone(A2, 1000, 200); // 4000Hz na pinie A2
    delay (200);
    tone(A2, 1000, 200); // 4000Hz na pinie A2
    delay (200);
    tone(A2, 1000, 200); // 4000Hz na pinie A2  
    digitalWrite(A1, HIGH);
    delay (5000);
    digitalWrite(A1, LOW);
    return;
  }
  
}
