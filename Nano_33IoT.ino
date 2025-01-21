#include "arduino_secrets.h"
#include "thingProperties.h"
#include <Keypad.h>

#define UART_TX 1 // TX pin
#define UART_RX 0 // RX pin

enum State {
  STANDBY,
  PASSWORD_ENTRY,
  ACCESS_GRANTED,
  ACCESS_DENIED
};
State currentState = STANDBY;

// Buzzer Pin
const int buzzerPin = A7;

// Ultrasonic Sensor Pins
const int trigPin = 9;
const int echoPin = 10;

// LED Pins
const int RledPin = 2; // Red LED pin
const int YledPin = 3; // Yellow LED pin
const int GledPin = 4; // Green LED pin

// Keypad setup
const byte ROWS = 4; // Number of rows on the keypad
const byte COLS = 4; // Number of columns on the keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3', '#'},
  {'4', '5', '6', '*'},
  {'7', '8', '9', '0'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A0, A1, A2, A3}; // Connect to the row pins of the keypad
byte colPins[COLS] = {5, 6, 7, 8};   // Connect to the column pins of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Correct password
const String correctPassword = "1234";
String inputPassword = "";

// Threshold for LED activation
const int distanceThreshold = 500;

// Timing variables
unsigned long lastFeedbackTime = 0;
const unsigned long feedbackInterval = 5000; // Send feedback every 5 seconds

unsigned long belowThresholdStart = 0;
const unsigned long belowThresholdDuration = 5000; // Time below threshold before LED lights up (5 seconds)

bool isBelowThreshold = false;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600); // Use Serial for debugging
  Serial1.begin(9600); // Use Serial1 for UART communication with Uno

  delay(1500);

  // Set up pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(RledPin, OUTPUT);
  pinMode(YledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  // Only open this if you want to do debugging in serial monitor (Rahul)
  // // Check and handle the current state
  //   switch (currentState) {
  //       case ACCESS_GRANTED:
  //           //Serial.println("Control State: Access Granted");
  //           break;
  //       case ACCESS_DENIED:
  //           //Serial.println("Control State: Access Denied");
  //           break;
  //       case STANDBY:
  //           //Serial.println("Control State: Standby");
  //           break;
  //   }

  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the echo response
  long duration = pulseIn(echoPin, HIGH);
  int distance = (duration * 0.0343) / 2;

  // Update `usonic` variable for IoT Cloud
  unsigned long currentTime = millis();
  if (currentTime - lastFeedbackTime >= feedbackInterval) {
    usonic = distance;
    Serial.print("Distance: ");
    Serial.println(distance);
    lastFeedbackTime = currentTime; // Reset feedback timer
  }

  // Handle LED and buzzer behavior based on distance
  if (distance <= distanceThreshold) {
    if (!isBelowThreshold) {
      belowThresholdStart = currentTime;
      isBelowThreshold = true;
    } else if (currentTime - belowThresholdStart >= belowThresholdDuration) {
      sendMessageToLCD("Motion Detected");
      currentState = ACCESS_DENIED; // Set to Access Denied
      digitalWrite(RledPin, HIGH);
      digitalWrite(YledPin, LOW);
      digitalWrite(GledPin, LOW);
      rLED = true; // Update the IoT Cloud variable
      yLED = false; // Update the IoT Cloud variable
      gLED = false; // Update the IoT Cloud variable
      ArduinoCloud.update();

      // Sound the alarm
      for (int i = 0; i < 5; i++) {
        tone(buzzerPin, 1000);
        delay(200);
        tone(buzzerPin, 1500);
        delay(200);
      }
      currentState = STANDBY; // Reset to standby state
      noTone(buzzerPin);
    }
  } else {

    
    isBelowThreshold = false;
    digitalWrite(YledPin, HIGH);
    digitalWrite(RledPin, LOW);
    digitalWrite(GledPin, LOW);
    
    yLED = true;
    rLED = false;
    gLED = false;
    noTone(buzzerPin);


    // Only display "Welcome Home!" if in STANDBY state
    if (currentState == STANDBY) {
      sendMessageToLCD("Welcome Home! by MicroOps (^_^)");
    }
  }


  
  // Handle keypad input
  char key = keypad.getKey();
  if (key) {
    tone(buzzerPin, 1000, 100);
    currentState = PASSWORD_ENTRY; // Switch to password entry state
    
    if (key == '#') {
      // Check password
      if (inputPassword == correctPassword) {
        currentState = ACCESS_GRANTED;
        tone(buzzerPin, 2000, 500); // Play a 1000Hz tone for 200 milliseconds
        delay(500);
        tone(buzzerPin, 1500, 200); // Play a 1000Hz tone for 200 milliseconds
        sendMessageToLCD("Access Granted");
        Serial.println("Access Granted");
        digitalWrite(RledPin, LOW);
        digitalWrite(YledPin, LOW);
        digitalWrite(GledPin, HIGH);
        gLED = true; // Update the IoT Cloud variable
        rLED = false;
        yLED = false;
        ArduinoCloud.update();
        delay(10000); // gives 10 seconds window for access granted
        currentState = STANDBY;
      } else {
        currentState = ACCESS_DENIED;
        sendMessageToLCD("Access Denied");
        tone(buzzerPin, 3500);
        delay(500);
        tone(buzzerPin, 500, 200);
        Serial.println("Access Denied");
        digitalWrite(GledPin, LOW);
        digitalWrite(RledPin, HIGH);
        digitalWrite(YledPin, LOW);
        gLED = false; // Update the IoT Cloud variable
        rLED = true;
        yLED = false;
        delay(1000);
        currentState = STANDBY; // Reset mode back to standby
      }
      inputPassword = ""; // Reset password
    } else if (key == '*') {
      inputPassword = ""; // Clear input
      sendMessageToLCD("Input Cleared");
      Serial.println("Input Cleared");
      delay(1000);
      currentState = STANDBY; // Reset mode back to standby
    } else {
      inputPassword += key; // Add key to input
      String maskedInput = "";
      for (unsigned int i = 0; i < inputPassword.length(); i++) {
          maskedInput += '*';
      }
      Serial.print("Password: ");
      sendMessageToLCD("Password: " + maskedInput); // Send the masked input to the LCD
      Serial.println(maskedInput); // Print the masked input for debugging (optional)
      Serial.println(inputPassword);
    }
  }
  
  delay(100); // Short delay for smoother updates
}


void onGLEDChange() {
  digitalWrite(GledPin, gLED ? HIGH: LOW); // Sync green LED with IoT Cloud command
}

void onRLEDChange() {
  digitalWrite(RledPin, rLED ? HIGH : LOW); // Sync red LED with IoT Cloud command
}

void onYLEDChange() {
  digitalWrite(YledPin, yLED ? HIGH : LOW); // Sync yellow LED with IoT Cloud command
}

void sendMessageToLCD(const String &message) {
  Serial1.print(message);
  Serial1.print('\n'); // Add a newline to indicate the end of the message
}

/*
  Since ControlState is READ_WRITE variable, onControlStateChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onControlStateChange()  {
   if (controlState) {
        // Set to ACCESS_GRANTED state
        currentState = ACCESS_GRANTED;

        // Example actions for "Access Granted"
        sendMessageToLCD("Access Granted (Cloud)");
        tone(buzzerPin, 2000, 500); // Play a 1000Hz tone for 200 milliseconds
        delay(500);
        tone(buzzerPin, 1500, 200); // Play a 1000Hz tone for 200 milliseconds
        digitalWrite(GledPin, HIGH); // Turn on green LED
        digitalWrite(RledPin, LOW);  // Turn off red LED
        digitalWrite(YledPin, LOW);  // Turn off yellow LED
        gLED = true; // Update the IoT Cloud variable
        rLED = false;
        yLED = false;
        ArduinoCloud.update();
        delay(10000); // Keep access open for 10 seconds
        currentState = STANDBY; // Return to standby
        controlState = !controlState; // Resets the Access Switch
        sendMessageToLCD("Armed Mode (Cloud)");
        tone(buzzerPin, 4000, 150); // High-pitched beep
        delay(150);
        tone(buzzerPin, 3000, 150); // Slightly lower-pitched beep
        delay(150);
        tone(buzzerPin, 3500, 200); // Final beep indicating arming
        delay(1000); // Short delay before resetting
    } else {
        // Set to Standby state
        currentState = STANDBY;

        // Example actions for "Access Denied"
        sendMessageToLCD("Armed Mode (Cloud)");
        tone(buzzerPin, 4000, 150); // High-pitched beep
        delay(150);
        tone(buzzerPin, 3000, 150); // Slightly lower-pitched beep
        delay(150);
        tone(buzzerPin, 3500, 200); // Final beep indicating arming
        digitalWrite(GledPin, LOW);  // Turn off green LED
        digitalWrite(RledPin, LOW); // Turn off red LED
        digitalWrite(YledPin, HIGH);  // Turn on yellow LED
        gLED = false; // Update the IoT Cloud variable
        rLED = false;
        yLED = true;
        ArduinoCloud.update();
        delay(1000); // Short delay before resetting
        currentState = STANDBY; // Return to standby
    }
}
