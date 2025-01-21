#include <LiquidCrystal.h>

// Define LCD pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
String receivedMessage = ""; // To store incoming messages

void setup() {
  lcd.begin(16, 2);         // Initialize the LCD
  lcd.print("Waiting...");  // Default message
  Serial.begin(9600);       // Initialize UART at 9600 baud rate

}

void loop() {
  // Check if data is available from the Nano
  if (Serial.available() > 0) {
    char c = Serial.read(); // Read one character
    if (c == '\n') {        // End of message (newline character)
      lcd.clear();
      displayMessage(receivedMessage); // Handle long messages
      

      receivedMessage = ""; // Clear the buffer
    } else {
      receivedMessage += c; // Append to the buffer
    }
  }
}

// Function to display message on LCD
void displayMessage(const String &message) {
  if (message.length() > 16) {
    // If the message exceeds 16 characters, split it into two lines
    String firstLine = message.substring(0, 16);  // First 16 characters
    String secondLine = message.substring(16);   // Remaining characters

    lcd.setCursor(0, 0);
    lcd.print(firstLine);

    lcd.setCursor(0, 1);
    lcd.print(secondLine);
  } else {
    // If the message fits on one line, display it on the first line
    lcd.setCursor(0, 0);
    lcd.print(message);
  }
}
