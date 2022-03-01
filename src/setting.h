#ifndef SETTING_H
#define SETTING_H

#include <Arduino.h>
#include <globaldef.h>
#include <prototypes.h>

extern Servo servoPan;
extern Servo servoTilt;
// extern Verify wifi;
// extern Verify blynk;

void Comunication(int baud_rate)
{
    //  Serial.begin(9600); //se #define BLYNK_PRINT Serial
    Serial.begin(baud_rate);
    String process = "| BOOT]";
    Serial.println("\n[" + String(millis()) + process + String(DEVICE_NAME));
}

void Setting_Pins()
{
    Input_Pins();
    Output_Pins();
    Initialize_Variables();
    Setting_Servo();
}

void Input_Pins()
{
    // pinMode(LED_PIN, INPUT);
    String process = "| BOOT] ";
    String step = "Setting > INPUT_PINS";
    Serial.println("[" + String(millis()) + process + step);
}

void Output_Pins()
{
    // pinMode(LED_PIN, OUTPUT);
    // Serial.println("outputpins();");
    // pinMode(OUTPIN_SERVO_PAN, OUTPUT);
    // pinMode(OUTPIN_SERVO_TILT, OUTPUT);

    String process = "| BOOT] ";
    String step = "Setting > OUTPUT_PINS";
    Serial.println("[" + String(millis()) + process + step);
}

void Initialize_Variables()
{
    // Serial.println("setFlags();");
    // wifi.ConectionState = NOT_CONNECTED_ST;        // indica o stado da conexão
    // blynk.ConectionState = BLYNK_NOT_CONNECTED_ST; // indica o stado da conexão com blynk
    String process = "| BOOT] ";
    String step = "Setting > INIT_VARIABLES";
    Serial.println("[" + String(millis()) + process + step);
}

void Setting_Servo()
{
    servoPan.attach(OUTPIN_SERVO_PAN);
    servoTilt.attach(OUTPIN_SERVO_TILT);
    String process = "| BOOT] ";
    String step = "Setting > SETTING_SERVO";
    Serial.println("[" + String(millis()) + process + step);
}
#endif