// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char SSID[]     = SECRET_SSID;    // Network SSID (name)
const char PASS[]     = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)

void onControlStateChange();
void onGLEDChange();
void onRLEDChange();
void onYLEDChange();

CloudSwitch controlState;
int usonic;
bool gLED;
bool rLED;
bool yLED;

void initProperties(){

  ArduinoCloud.addProperty(controlState, READWRITE, ON_CHANGE, onControlStateChange);
  ArduinoCloud.addProperty(usonic, READ, ON_CHANGE, NULL, 500);
  ArduinoCloud.addProperty(gLED, READWRITE, ON_CHANGE, onGLEDChange);
  ArduinoCloud.addProperty(rLED, READWRITE, ON_CHANGE, onRLEDChange);
  ArduinoCloud.addProperty(yLED, READWRITE, ON_CHANGE, onYLEDChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
