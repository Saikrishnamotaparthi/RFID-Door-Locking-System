#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

Servo myServo;

byte authorizedUID[4] = {0xDE, 0xAD, 0xBE, 0xEF}; // Change this to your tag's UID

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  myServo.attach(3);
  myServo.write(0); // Locked position
  Serial.println("RFID Door Lock System Ready");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  Serial.print("Card UID: ");
  bool authorized = true;
  for (byte i = 0; i < 4; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    if (rfid.uid.uidByte[i] != authorizedUID[i]) authorized = false;
  }
  Serial.println();

  if (authorized) {
    Serial.println("Access Granted");
    myServo.write(90); // Unlock
    delay(3000);
    myServo.write(0);  // Lock again
  } else {
    Serial.println("Access Denied");
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
