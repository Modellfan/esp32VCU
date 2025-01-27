#include <ESP32-TWAI-CAN.hpp>
#include <params.h>
#include <param_save.h>
#include <FS.h>
#include <SPIFFS.h>
#include <Arduino.h>
#include "cansdo.h"

// Default for ESP32
#define CAN_TX 21
#define CAN_RX 20

#define DESIG_UNIQUE_ID0 ((uint32_t)(ESP.getEfuseMac() & 0xFFFFFFFF))         // Lower 32 bits
#define DESIG_UNIQUE_ID1 ((uint32_t)((ESP.getEfuseMac() >> 32) & 0xFFFFFFFF)) // Upper 32 bits
#define DESIG_UNIQUE_ID2 (0x00000000)                                         // ESP32 does not have a 96-bit unique ID, so this is 0

CanSdo sdo;

void PrintList()
{
    Serial.println("Available parameters and values:");

    for (uint32_t idx = 0; idx < Param::PARAM_LAST; idx++)
    {
        const Param::Attributes *pAtr = Param::GetAttrib((Param::PARAM_NUM)idx);

        if ((Param::GetFlag((Param::PARAM_NUM)idx) & Param::FLAG_HIDDEN) == 0)
        {
            Serial.print(pAtr->name);
            Serial.print(" [");
            Serial.print(pAtr->unit);
            Serial.println("]");
        }
    }
}

void PrintParamsJson()
{
    Serial.println("{");
    char comma = ' '; // Tracks whether to print a comma

    for (uint32_t idx = 0; idx < Param::PARAM_LAST; idx++)
    {
        const Param::Attributes *pAtr = Param::GetAttrib((Param::PARAM_NUM)idx);

        // Always include parameters (hidden or not)
        Serial.print(comma);
        Serial.println();
        Serial.print("   \"");
        Serial.print(pAtr->name);
        Serial.print("\": {\"unit\":\"");
        Serial.print(pAtr->unit);
        Serial.print("\",\"id\":");
        Serial.print(pAtr->id);
        Serial.print(",\"value\":");
        Serial.print(Param::Get((Param::PARAM_NUM)idx));
        Serial.print(",");

        // Include parameter type and additional attributes
        if (Param::GetType((Param::PARAM_NUM)idx) == Param::TYPE_PARAM ||
            Param::GetType((Param::PARAM_NUM)idx) == Param::TYPE_TESTPARAM)
        {
            Serial.print("\"isparam\":true,\"minimum\":");
            Serial.print(pAtr->min);
            Serial.print(",\"maximum\":");
            Serial.print(pAtr->max);
            Serial.print(",\"default\":");
            Serial.print(pAtr->def);
            Serial.print(",\"category\":\"");
            Serial.print(pAtr->category);
            Serial.print("\",\"i\":");
            Serial.print(idx);
            Serial.print("}");
        }
        else
        {
            Serial.print("\"isparam\":false}");
        }

        comma = ','; // Add a comma after the first parameter
    }

    // Add a unique identifier as part of the JSON object
    Serial.println(",");
    Serial.print("   \"serial\": {\"unit\":\"\",\"value\":\"");
    Serial.print("12345678"); // Replace "12345678" with actual unique ID logic if available
    Serial.println("\",\"isparam\":false}");
    Serial.println("}");
}

void sendObdFrame(uint8_t obdId)
{
    CanFrame obdFrame = {0};
    obdFrame.identifier = 0x7DF; // Default OBD2 address;
    obdFrame.extd = 0;
    obdFrame.data_length_code = 8;
    obdFrame.data[0] = 2;
    obdFrame.data[1] = 1;
    obdFrame.data[2] = obdId;
    obdFrame.data[3] = 0xAA; // Best to use 0xAA (0b10101010) instead of 0
    obdFrame.data[4] = 0xAA; // CAN works better this way as it needs
    obdFrame.data[5] = 0xAA; // to avoid bit-stuffing
    obdFrame.data[6] = 0xAA;
    obdFrame.data[7] = 0xAA;
    // Accepts both pointers and references
    ESP32Can.writeFrame(obdFrame); // timeout defaults to 1 ms
}

void Param::Change(Param::PARAM_NUM paramNum)
{
    switch (paramNum)
    {
    default:
        // Handle general parameter changes here. Add paramNum labels for handling specific parameters
        break;
    }
}

File jsonParamFile; // Global file object for params.json

bool initializeSPIFFSAndLoadFile()
{
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
        Serial.println("Failed to mount SPIFFS");
        return false;
    }

    // Open the params.json file
    jsonParamFile = SPIFFS.open("/params.json", FILE_READ);
    if (!jsonParamFile)
    {
        Serial.println("Failed to open params.json for reading.");
        SPIFFS.end(); // Unmount SPIFFS before returning
        return false;
    }
    return true;
}

void PrintParamsJsonfromfile()
{
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    { // Mount SPIFFS (format if necessary)
        Serial.println("Failed to mount SPIFFS");
        return;
    }

    // Open the file for reading
    File file = SPIFFS.open("/params.json", FILE_READ);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        SPIFFS.end();
        return;
    }

    // Read the contents of the file and print them line by line
    Serial.println("Contents of params.json:");
    while (file.available())
    {
        Serial.write(file.read());
    }

    Serial.println(); // Add a newline after printing the file contents
    file.close();
    SPIFFS.end(); // Unmount SPIFFS
}

// Function to write all parameters to a JSON file, always including hidden ones
void WriteParamsJsonToFlash()
{
    const Param::Attributes *pAtr;

    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    { // Format if necessary
        Serial.println("Failed to mount SPIFFS");
        return;
    }

    // Open the file for writing
    File file = SPIFFS.open("/params.json", FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        SPIFFS.end();
        return;
    }

    // Start JSON
    file.print("{\r\n");

    // Iterate through parameters
    bool firstParam = true; // Flag to track the first parameter
    for (uint32_t idx = 0; idx < Param::PARAM_LAST; idx++)
    {
        uint32_t canId;
        uint8_t canStart;
        int8_t canLength, offset;
        bool isRx;
        float canGain;

        pAtr = Param::GetAttrib((Param::PARAM_NUM)idx);

        // Add a comma only if it's not the first parameter
        if (!firstParam)
        {
            file.print(",\r\n");
        }
        firstParam = false;

        // Write parameter information
        file.print("   \"");
        file.print(pAtr->name);
        file.print("\": {\"unit\":\"");
        file.print(pAtr->unit);
        file.print("\",\"id\":");
        file.print(pAtr->id);
        file.print(",\"value\":");
        file.print(Param::Get((Param::PARAM_NUM)idx));
        file.print(",");

        // Include parameter type and additional attributes
        if (Param::GetType((Param::PARAM_NUM)idx) == Param::TYPE_PARAM ||
            Param::GetType((Param::PARAM_NUM)idx) == Param::TYPE_TESTPARAM)
        {
            file.print("\"isparam\":true,\"minimum\":");
            file.print(pAtr->min);
            file.print(",\"maximum\":");
            file.print(pAtr->max);
            file.print(",\"default\":");
            file.print(pAtr->def);
            file.print(",\"category\":\"");
            file.print(pAtr->category);
            file.print("\",\"i\":");
            file.print(idx);
            file.print("}");
        }
        else
        {
            file.print("\"isparam\":false}");
        }
    }

    // Add the unique identifier
    uint32_t uniqueID0 = DESIG_UNIQUE_ID0; // Lower 32 bits
    uint32_t uniqueID1 = DESIG_UNIQUE_ID1; // Upper 32 bits
    uint32_t uniqueID2 = DESIG_UNIQUE_ID2; // Placeholder for 0

    if (!firstParam)
    {
        file.print(",\r\n");
    }
    file.print("   \"serial\": {\"unit\":\"\",\"value\":\"");
    file.printf("%08X%08X%08X", uniqueID1, uniqueID0, uniqueID2); // Concatenate the IDs
    file.print("\",\"isparam\":false}");
    file.print("\r\n}");

    file.close(); // Close the file
    SPIFFS.end(); // Unmount SPIFFS

    Serial.println("JSON file written successfully to flash with all parameters, including hidden ones");
}

void setup()
{
    // Setup serial for debbuging.
    Serial.begin(115200);

    // PrintParamsJson();

    //   // Set pins
    // ESP32Can.setPins(CAN_TX, CAN_RX);

    //   // You can set custom size for the queues - those are default
    //   ESP32Can.setRxQueueSize(5);
    // ESP32Can.setTxQueueSize(5);

    //   // .setSpeed() and .begin() functions require to use TwaiSpeed enum,
    //   // but you can easily convert it from numerical value using .convertSpeed()
    //   ESP32Can.setSpeed(ESP32Can.convertSpeed(500));

    //   // You can also just use .begin()..
    //   if(ESP32Can.begin()) {
    //       Serial.println("CAN bus started!");
    //   } else {
    //       Serial.println("CAN bus failed!");
    //   }

    // or override everything in one command;
    // It is also safe to use .begin() without .end() as it calls it internally

    WriteParamsJsonToFlash();
    // PrintParamsJsonfromfile();
    if (!initializeSPIFFSAndLoadFile())
    {
        Serial.println("Failed to initialize SPIFFS or load parameter file.");
    }
    else
    {
        Serial.println("SPIFFS initialized and params.json loaded successfully.");
    }

    Param::SetInt(Param::version, 4); // COM protocol version 4

    if (ESP32Can.begin(ESP32Can.convertSpeed(500), CAN_TX, CAN_RX, 10, 10))
    {
        Serial.println("CAN bus started!");
    }
    else
    {
        Serial.println("CAN bus failed!");
    }
}

void loop()
{
    CanFrame rxFrame;
    static uint32_t lastStamp = 0;
    uint32_t currentStamp = millis();

    if (currentStamp - lastStamp > 1000)
    { // sends OBD2 request every second
        lastStamp = currentStamp;
        // sendObdFrame(5); // For coolant temperature
        // Serial.printf("Sent");
    }

    // You can set custom timeout, default is 1000
    if (ESP32Can.readFrame(rxFrame, 1000))
    {
        // Comment out if too many requests
        // Serial.printf("Received frame: %03X \r\n", rxFrame.identifier);
        sdo.HandleRx(&rxFrame);
    }
}