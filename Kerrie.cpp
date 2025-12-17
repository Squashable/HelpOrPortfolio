#include <Servo.h>
#include <NewPing.h>

// ----- LED Pins -----
#define PIN_PWR       5
#define PIN_FULL_L    6
#define PIN_FULL_R    7
#define PIN_YELLOW    4

// ----- Ultrasonic -----
#define SONAR_NUM      2
#define MAX_DISTANCE  100

NewPing Sonar[SONAR_NUM] = {
  NewPing(A1, A0, MAX_DISTANCE),
  NewPing(A3, A2, MAX_DISTANCE)
};

// ----- Servo -----
Servo myservo;

// ----- Buttons -----
#define BTN1_PIN 2
#define BTN2_PIN 3

// ----- Servo Timing -----
bool ServoSpin = false;
unsigned long servoTimer = 0;
const unsigned long ServoMoveTime = 1000; // ms

// ----- Button Debounce -----
const unsigned long debounceDelay = 50;

bool btn1State = HIGH;
bool btn2State = HIGH;
bool lastBtn1Reading = HIGH;
bool lastBtn2Reading = HIGH;
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;

void setup() {
  myservo.attach(10);
  myservo.write(90); // default position

  pinMode(PIN_PWR, OUTPUT);
  pinMode(PIN_FULL_L, OUTPUT);
  pinMode(PIN_FULL_R, OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  digitalWrite(PIN_PWR, HIGH);

  Serial.begin(9600);
}

void loop() {
  // --- Read Buttons with Debounce ---
  bool reading1 = digitalRead(BTN1_PIN);
  bool reading2 = digitalRead(BTN2_PIN);

  // Button 1 debounce
  if (reading1 != lastBtn1Reading) lastDebounceTime1 = millis();
  if (millis() - lastDebounceTime1 > debounceDelay) btn1State = reading1;
  lastBtn1Reading = reading1;

  // Button 2 debounce
  if (reading2 != lastBtn2Reading) lastDebounceTime2 = millis();
  if (millis() - lastDebounceTime2 > debounceDelay) btn2State = reading2;
  lastBtn2Reading = reading2;

  // Active LOW buttons
  bool btn1Pressed = (btn1State == LOW);
  bool btn2Pressed = (btn2State == LOW);

  // --- Ultrasonic sensors ---
  int distance1 = Sonar[0].ping_cm();
  delay(30);
  int distance2 = Sonar[1].ping_cm();
  delay(30);

  Serial.print("Left: ");
  Serial.print(distance1);
  Serial.print(" cm | Right: ");
  Serial.print(distance2);
  Serial.println(" cm");

  // --- Servo Control ---
  if (!ServoSpin) {
    if (btn1Pressed) {
      myservo.write(0);
      ServoSpin = true;
      servoTimer = millis();
      digitalWrite(PIN_YELLOW, HIGH);
      Serial.println("Button 1 pressed");
    }
    else if (btn2Pressed) {
      myservo.write(180);
      ServoSpin = true;
      servoTimer = millis();
      digitalWrite(PIN_YELLOW, HIGH);
      Serial.println("Button 2 pressed");
    }
  }

  // Return servo & unlock buttons
  if (ServoSpin && millis() - servoTimer >= ServoMoveTime) {
    myservo.write(90);
    ServoSpin = false;
    digitalWrite(PIN_YELLOW, LOW);
    Serial.println("Servo reset");
  }

  // --- Bin Full LEDs ---
  digitalWrite(PIN_FULL_L, (distance1 > 0 && distance1 < 15));
  digitalWrite(PIN_FULL_R, (distance2 > 0 && distance2 < 15));
}
