/*
   Software desenvolvido para controle de abertura/fechamento de
   portão eletronico pelo smartphone usando a plataforma Blynk

   ESP-01  IDE
   GPIO0    0
   TXD      1
   GPIO2    2
   RXD      3

    WiFi.status()
    WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
    WL_IDLE_STATUS      = 0,
    WL_NO_SSID_AVAIL    = 1,
    WL_SCAN_COMPLETED   = 2,
    WL_CONNECTED        = 3,
    WL_CONNECT_FAILED   = 4,
    WL_CONNECTION_LOST  = 5,
    WL_DISCONNECTED     = 6
*/
#include <Arduino.h>
#include <TimeLib.h>
#include <BlynkSimpleEsp32.h>
// #include <BlynkSimpleEsp8266.h>

#include <WidgetRTC.h>

//#include <ModbusIP_ESP8266.h>
#include <DNSServer.h>

// #include <ESP8266WebServer.h>

#include <connection.h>
#include <globaldef.h>
// #include <blynkLED.h>
#include <memoriaFlash.h>
#include <setupOTA.h>
#include <setupBlynk.h>
#include <saveStatus.h>
#include <isr.h>
#include <setting.h>
#include <prototypes.h>

bool bOffLineMode = false;
bool f_SerialOut_ON = true;

int intLedOn = LED_STATICO_ON;  // tempo em que o led de status fica on
int intLedOff = LED_STATICO_ON; // tempo em que o led de status fica off
bool bLedState = false;         // indica se o led esta on/off

struct Verify wifi;
struct Verify blynk;
struct ESP_32 MyESP32;

unsigned long ulTimeVerify = 0; // variavel de controle

bool bBlynkButtonState = false;
bool bKeyPulseState = false;
bool bReleState = false;

bool lock_button = true;

const char esp32_rede[20] = DEVICE_NAME;
const char esp32_pass[20] = DEVICE_PASS;
char blynk_token[34] = "YOUR_BLYNK_TOKEN";

hw_timer_t *timer = NULL;

bool bTouchState = false;
bool f_Reconnect_WIFI = false;
bool f_Reconnect_Blynk = false;

unsigned long TimeCheck = 0;
unsigned long TimeOutConnect = 0;
unsigned long TimeOut = 0;
// void inite(uint8_t chg_pin);

WidgetTerminal terminal(V4);

void setup()
{
  Comunication(BAUD_RATE);

  Setting_Pins();

  Start_Timer(TIME_INTERRUPT);

  MyESP32.ConnectedWiFi = Assistant_WiFi();

  if (MyESP32.ConnectedWiFi)
  {
    int result = Assistant_Blynk();
    if (result == SUCCESS_)
    {
      MyESP32.ConnectedBlynk = true;
    }
    else if ((result == ERRO_) || result == INVALID_TOKEN)
    {
      MyESP32.ConnectedBlynk = false;
    }
    else
    {
      MyESP32.ConnectedBlynk = false;
      MyESP32.ConnectedWiFi = false;
    }
  }

  Programming_OTA();

  TimeCheck = TIME_CHECK_CONNECTION;
}

void loop()
{
  if (MyESP32.CheckConnection)
  {
    MyESP32.ConnectedBlynk = Blynk.connected();
    MyESP32.ConnectedWiFi = WiFi_Connected();

    if (!MyESP32.ConnectedWiFi)
    {
      MyESP32.ConnectedWiFi = Assistant_WiFi();
    }

    if (!MyESP32.ConnectedBlynk && MyESP32.ConnectedWiFi)
    {
      int result = Assistant_Blynk();
      if (result == SUCCESS_)
      {
        MyESP32.ConnectedBlynk = true;
      }
      else if ((result == ERRO_) || result == INVALID_TOKEN)
      {
        MyESP32.ConnectedBlynk = false;
      }
      else
      {
        MyESP32.ConnectedBlynk = false;
        MyESP32.ConnectedWiFi = false;
      }
    }

    MyESP32.CheckConnection = false;
    TimeCheck = TIME_CHECK_CONNECTION; // reload timer
    // Serial.println(".");
  }

  if (MyESP32.ConnectedBlynk)
  {
    Blynk.run();
  }

  ArduinoOTA.handle();
}

BLYNK_WRITE(RESET_WIFI_V1)
{
  int value = param.asInt(); // Get value as integer
  if (value)
  {
    WiFiManager wifiConnect;
    wifiConnect.resetSettings();
    WiFi.disconnect();
    TimeCheck = 1;
    // SPIFFS.format();
    // ESP.restart();
  }
}

// BLYNK_WRITE(LOCK_BUTTON_V3)
//{
//   int value = param.asInt(); // Get value as integer
//
//   if (value)
//   {
//     lock_button = true;
//   }
//   else
//   {
//     lock_button = false;
//   }
// }

// BLYNK_WRITE(ON_OFF_V2)
// {
//   Serial.println("\nBLYNK_WRITE\n");
//   int value = param.asInt(); // Get value as integer

//   if (value)
//   {
//     digitalWrite(RELE_PIN, false); // ligo o rele
//     bBlynkButtonState = true;      // atualizo o status do button blynk
//     bReleState = true;             // atualizo o status do rele
//   }
//   else
//   {
//     digitalWrite(RELE_PIN, true); // ligo o rele
//     bBlynkButtonState = false;    // atualizo o status do button blynk
//     bReleState = false;           // atualizo o status do rele
//   }

//   saveStatus();
// }

// BLYNK_CONNECTED()
// {
//   Serial.println("\nBLYNK_CONNECTED SYNC\n");
//   //  rtc.begin();

//   // caso a lampada tenha sido acionada pela chave de pulso
//   if (bReleState && bBlynkButtonState)
//   {
//     Blynk.virtualWrite(ON_OFF_V2, 1); // altero o status do butão para on
//   }
//   else
//   {
//     Blynk.syncVirtual(ON_OFF_V2);
//   }
// }

// void myTimerEvent()
//{
//   if (!(Blynk.connected()))
//   {
//     ESP.restart();
//   }
//
//   digitalWrite(LED_, stsLED);
//   stsLED = !stsLED;
//   blynkLED += 1;
//
//   if (blynkLED > TIME_LED_OFF)
//   {
//     pinMode(LED_, OUTPUT);
//     digitalWrite(LED_, false);
//     blynkLED = 0;
//   } else if (blynkLED == TIME_LED_ON)
//   {
//     pinMode(LED_, OUTPUT);
//     digitalWrite(LED_, true);
//   }
//
// }
