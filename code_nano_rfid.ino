#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;

void setup() {
  lcd.begin();
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

}

void loop() {
  lcd.setCursor(0,0);
  lcd.print(" Put Your Card ");
  lcd.setCursor(5,1);
  lcd.print(" Here ");
  delay(1000);
  lcd.clear();
  readsuccess = getid();
  String s = StrUID;
  if (readsuccess) {
    Serial.write(str);
    lcd.setCursor(0,0);
    lcd.print(" MARKED SUCCESS ");
    lcd.setCursor(1,1);
    lcd.print("ID = ");
    lcd.setCursor(7,1);
    lcd.print(str);
    delay(4000);
    lcd.clear();
  }
}

int getid() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }

  for (int i = 0; i < 4; i++) {
    readcard[i] = mfrc522.uid.uidByte[i]; //storing the UID of the tag in readcard
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();
  return 1;
}

void array_to_string(byte array[], unsigned int len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len * 2] = '\0';
}
