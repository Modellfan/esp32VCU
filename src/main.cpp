#include <Arduino.h>
#include "USB.h"
#include "USBCDC.h"

// Create two CDC objects
USBCDC USBSerial1(0); // First virtual serial port

void setup() {
    Serial.begin(115200);
    
    USBSerial1.begin();
    USB.begin(); // Start USB
}

void loop() {
    // Read from USBSerial1 and forward to USBSerial2
    if (USBSerial1.available()) {
        String data = USBSerial1.readString();
        Serial.print("USBSerial1 received: ");
        Serial.println(data);
    }

    delay(100);
}

