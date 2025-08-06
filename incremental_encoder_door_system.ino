#include <Arduino.h>
#include <Encoder.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define motor control pins
const int motorEnablePin = 9;
const int motorPin1 = 8;
const int motorPin2 = 7;

int heartLED = 12;
int openLedGRN = 11;
int closeLedYLW = 13;

// Define encoder pins
const int encoderPin1 = 3;
const int encoderPin2 = 4;

// Define PIR sensor pin
const int pirSensorPin = 2;

// Create encoder instance
Encoder encoder(encoderPin1, encoderPin2);

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Number of encoder pulses per revolution (adjust according to your encoder)
const int pulsesPerRevolution = 1000;

// Desired number of turns
const int desiredTurns = 1;

// Timing variables
unsigned long previousMillis = 0;
const long interval = 5000; // 5 seconds

// State variables
enum State {STOPPED, COUNTERCLOCKWISE, WAIT, CLOCKWISE};
State currentState = STOPPED;

bool beatingHeart = LOW;
unsigned long blinkMillis = 0;

void setup() {
  Serial.begin(9600);

  pinMode(heartLED, OUTPUT);
  pinMode(openLedGRN, OUTPUT);
  pinMode(closeLedYLW, OUTPUT);

  // Set up motor control pins
  pinMode(motorEnablePin, OUTPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  // Set up PIR sensor pin
  pinMode(pirSensorPin, INPUT);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.display();

  // Reset encoder position
  encoder.write(0);

  Serial.println("Setup complete. Starting loop.");
}

void loop() {
  motorControl();
  heartBeat();
}

void motorControl() {
  static long targetPosition = pulsesPerRevolution * desiredTurns;
  long currentPosition = encoder.read();
  unsigned long currentMillis = millis();
  bool motionDetected = digitalRead(pirSensorPin) == HIGH;

  switch (currentState) {
    case STOPPED:
      stop();
      if (motionDetected) {
        encoder.write(0);
        currentState = COUNTERCLOCKWISE;
        displayMessage("Door Opening", "Sensor ON", "DiaaEldin");
        digitalWrite(openLedGRN, HIGH);
        digitalWrite(closeLedYLW, LOW);
      }
      break;

    case COUNTERCLOCKWISE:
      counterClockwise();
      if (abs(currentPosition) >= targetPosition) {
        stop();
        currentState = WAIT;
        previousMillis = currentMillis;
        displayMessage("Door Is Open", "Please Pass Through", "DiaaEldin");
        digitalWrite(openLedGRN, LOW);
        digitalWrite(closeLedYLW, LOW);
      }
      break;

    case WAIT:
      if (currentMillis - previousMillis >= interval) {
        encoder.write(0);
        currentState = CLOCKWISE;
        displayMessage("Door Closing", "Please Stand Clear", "DiaaEldin");
        digitalWrite(openLedGRN, LOW);
        digitalWrite(closeLedYLW, HIGH);
      }
      break;

    case CLOCKWISE:
      clockwise();
      if (abs(currentPosition) >= targetPosition) {
        stop();
        currentState = STOPPED;
        displayMessage("Door Closed", "Sensor OFF", "DiaaEldin");
        digitalWrite(openLedGRN, LOW);
        digitalWrite(closeLedYLW, LOW);
      }
      break;
  }

  // Debugging and display updates
  Serial.print("Encoder position: ");
  Serial.println(currentPosition);
}

void clockwise() {
  analogWrite(motorEnablePin, 255); // Full speed
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  Serial.println("Motor turning clockwise");
}

void counterClockwise() {
  analogWrite(motorEnablePin, 255); // Full speed
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  Serial.println("Motor turning counterclockwise");
}

void stop() {
  analogWrite(motorEnablePin, 0);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  Serial.println("Motor stopped");
}

void displayMessage(const char* message1, const char* message2, const char* message3) {
  display.clearDisplay();

  // First line
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print(message1);

  // Second line
  display.setCursor(0, 20);
  display.setTextSize(1);
  display.print(message2);

  // Third line
  display.setCursor(0, 40);
  display.setTextSize(1);
  display.print(message3);

  // Display everything
  display.display();
}

void heartBeat() {
  unsigned long currentMillis = millis();
  if (currentMillis - blinkMillis >= 100) {
    blinkMillis = currentMillis;
    beatingHeart = !beatingHeart;
    digitalWrite(heartLED, beatingHeart ? HIGH : LOW);
  }
}
