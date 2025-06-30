#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Servo.h>

// === Components ===
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;
Servo servo1; // Rotates every minute
Servo servo2; // Dispenses pill on hand detection

// === Pins ===
const int irPin = 6;
const int buzzerPin = 8;
const int servo1Pin = 11;
const int servo2Pin = 12;

// === Variables ===
int pillCount = 4;
unsigned long lastServo1Time = 0;
unsigned long lastDispenseTime = 0;
const unsigned long interval = 60UL * 1000UL; // 1 minute in milliseconds

// === Servo1 Rotation Positions ===
int servo1Positions[] = {180, 135, 90, 45};
int servo1Index = 0;

// === Pill Icon (custom character) ===
uint8_t pillChar[8] = {
  B00000,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00000,
  B00000
};

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, pillChar);

  if (!rtc.begin()) {
    lcd.print("RTC not found!");
    while (1);
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(irPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(servo1Positions[servo1Index]);
  servo2.write(0);

  lcd.clear();
  lcd.print("Dispenser Ready!");
  delay(2000);
  lcd.clear();

  lastServo1Time = millis();
  lastDispenseTime = millis() - interval;
}

void loop() {
  DateTime now = rtc.now();
  unsigned long currentMillis = millis();

  // === Display Time ===
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  printTime(now);

  // === Rotate Servo1 Every Minute ===
  if (currentMillis - lastServo1Time >= interval) {
    servo1Index = (servo1Index + 1) % 4;
    servo1.write(servo1Positions[servo1Index]);
    lastServo1Time = currentMillis;
  }

  // === IR Sensor + Dispense Logic ===
  int irVal = digitalRead(irPin);
  lcd.setCursor(0, 1);

  if (pillCount == 0) {
    lcd.print("Refill Pills Now ");
  } else if (irVal == LOW && currentMillis - lastDispenseTime >= interval) {
    digitalWrite(buzzerPin, HIGH);
    lcd.print("Pill Out... ");
    lcd.write(byte(0));
    delay(500);
    servo2.write(55);
    delay(500);
    servo2.write(0);
    digitalWrite(buzzerPin, LOW);

    pillCount--;
    lastDispenseTime = currentMillis;
    delay(1000);
  } else if (irVal == LOW && currentMillis - lastDispenseTime < interval) {
    lcd.print("Wait... 1 min ");
    lcd.write(byte(0));
  } else {
    lcd.print("Ready ");
    lcd.write(byte(0));
    lcd.print(" Left:");
    lcd.print(pillCount);
  }

  delay(200);
}

// === Helper: Print Time ===
void printTime(DateTime now) {
  if (now.hour() < 10) lcd.print('0');
  lcd.print(now.hour());
  lcd.print(':');
  if (now.minute() < 10) lcd.print('0');
  lcd.print(now.minute());
  lcd.print(':');
  if (now.second() < 10) lcd.print('0');
  lcd.print(now.second());
}
