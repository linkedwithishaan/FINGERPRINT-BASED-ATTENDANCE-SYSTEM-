#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define BUZZER 2

void setup() {
  Serial.begin(115200);

  mySerial.begin(57600, SERIAL_8N1, 16, 17);

  finger.begin(57600);

  Wire.begin(21, 22);
  lcd.begin();
  lcd.backlight();

  pinMode(BUZZER, OUTPUT);

  lcd.print("Initializing...");

  if (finger.verifyPassword()) {
    lcd.clear();
    lcd.print("Sensor Ready");
  } else {
    lcd.clear();
    lcd.print("Sensor Error");
    while (1);
  }

  delay(2000);
  lcd.clear();
  lcd.print("Place Finger");
}

void loop() {
  int id = getFingerprintID();

  if (id != -1) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ID:");
    lcd.print(id);

    lcd.setCursor(0,1);
    lcd.print("Attendance OK");

    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);

    delay(2000);
    lcd.clear();
    lcd.print("Place Finger");
  }
}

int getFingerprintID() {
  if (finger.getImage() != FINGERPRINT_OK) return -1;
  if (finger.image2Tz() != FINGERPRINT_OK) return -1;
  if (finger.fingerFastSearch() != FINGERPRINT_OK) return -1;

  return finger.fingerID;
}
