# Smart-Hostel-Security-System

## Overview
The **Smart Hostel Security System** is an IoT-based solution designed to enhance security in hostel environments. The system integrates hardware components with cloud connectivity to provide real-time monitoring, access control, and instant notifications via multiple platforms.

## Features
- **Password-Protected Access:** Users can securely enter a password via a 4x4 matrix keypad.
- **Motion Detection:** Ultrasonic sensor detects unauthorized movement.
- **LED Indicators & Buzzer Alerts:** Visual and audible feedback for different security states.
- **IoT Cloud Integration:** Remote monitoring and control via the Arduino IoT Cloud.
- **Multi-Platform Notifications:** Instant alerts sent through WhatsApp, Telegram, Discord, and Email using Zapier webhooks.

## System Components
1. **Microcontrollers:**
   - Arduino Nano 33 IoT (main controller)
   - Arduino UNO R3 (for LCD display management)
2. **Sensors and Actuators:**
   - Ultrasonic sensor (motion detection)
   - 4x4 matrix keypad (password input)
   - LED indicators (Green, Red, Yellow)
   - Buzzer (alert notifications)
   - LCD display (system messages)
3. **Cloud Integration:**
   - Arduino IoT Cloud
   - Zapier for multi-platform notifications
4. **Additional Components:**
   - Potentiometer
   - Breadboard
   - Resistors (1K Ohm)
   - Jumper wires

## System Workflow
1. **Initialization:** System powers on and connects to the IoT Cloud.
2. **User Access:**
   - Correct password input grants access.
   - Incorrect password triggers alerts.
3. **Motion Detection:** Unauthorized movement triggers alerts and notifications.
4. **Remote Control:** Users can arm/disarm the system via the cloud dashboard.
5. **Real-Time Alerts:** Notifications are sent via WhatsApp, Telegram, Discord, and Email.

## Challenges Faced & Solutions Implemented
- **Component Shortages:** Some components like ESP32 Cam and magnetic sensors were unavailable; solutions involved optimizing available components.
- **Compatibility Issues:** PIR sensors were incompatible; replaced with ultrasonic sensors.
- **Pin Constraints:** Analog pins were repurposed to address limited digital pin availability.
- **Cloud Limitations:** Optimization of cloud variable usage due to free-tier restrictions.

## Achievements
The project received **full marks** for both the report and presentation, highlighting its innovation, functionality, and comprehensive documentation.

## Future Improvements
- Integration of additional sensors (e.g., cameras for visual surveillance).
- Enhanced cloud capabilities with a premium IoT cloud subscription.
- Improved notification system using Firebase Cloud Messaging.
- Implementation of a password reset feature via the cloud platform.

## How to Set Up
### Hardware Setup
1. Connect the Arduino Nano 33 IoT and Arduino UNO R3 to the breadboard.
2. Attach sensors and actuators (keypad, ultrasonic sensor, LEDs, buzzer, LCD display).
3. Ensure correct wiring and power connections.

### Software Setup
1. Install the Arduino IDE and required libraries.
2. Upload the provided code to the respective microcontrollers.
3. Configure the IoT Cloud dashboard and Zapier integrations.

## Usage Instructions
1. Power on the system.
2. Input the password via the keypad.
3. Monitor real-time alerts and control the system remotely through the cloud dashboard.

## Technologies Used
- **Programming Languages:** C++ (Arduino IDE)
- **Hardware Platforms:** Arduino Nano 33 IoT, Arduino UNO R3
- **Cloud Services:** Arduino IoT Cloud, Zapier

## Contributors
- [Your Name]
- Team MicroOps

## License
This project is licensed under the MIT License.

## Contact
For any inquiries, please contact: [your email]

