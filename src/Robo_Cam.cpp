/*
   Software desenvolvido para controle de abertura/fechamento de
   portão eletronico pelo smartphone usando a plataforma Blynk

   Software desenvolvido para controlar um robo suspenso em um cabo

*/

#include "prototypes.h"

bool bOffLineMode = false;
bool f_SerialOut_ON = true;

int intLedOn = LED_STATICO_ON;  // tempo em que o led de status fica on
int intLedOff = LED_STATICO_ON; // tempo em que o led de status fica off

bool bLedState = false; // indica se o led esta on/off
int auxLed = 0;         // variavel auxiliar do led

struct Verify wifi;
struct Verify blynk;
struct ESP_32 MyESP32;
struct RoboCan MyMoves;

unsigned long ulTimeVerify = 0; // variavel de controle

// bool bBlynkButtonState = false;
// bool bKeyPulseState = false;
// bool bReleState = false;

// bool lock_button = true;

// const char esp32_rede[20] = DEVICE_NAME;
// const char esp32_pass[20] = DEVICE_PASS;

const char esp32_rede[20] = "Game_Play_LanH";
const char esp32_pass[20] = "25061997";

// char blynk_token[35] = "YOUR_BLYNK_TOKEN";

char blynk_token[35] = "m7ia9_0Io8qtIThzcfq3xxZ8_gm61Q17";
char blynk_server[17] = "YOUR_SERVER";
char blynk_port[5] = "PORT";

hw_timer_t *timer = NULL;

// bool bTouchState = false;
// bool f_Reconnect_WIFI = false;
// bool f_Reconnect_Blynk = false;

unsigned long TimeCheck = 0;
unsigned long TimeOutConnect = 0;
unsigned long TimeOut = 0;
unsigned long CountTime = 0;

String output2State;

// Servo servoPan;
// Servo servoTilt;

WidgetTerminal terminal(V10);

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;

void setup()
{
  Comunication(BAUD_RATE);

  Setting_Pins();

  engine.init();
  stepper = engine.stepperConnectToPin(OUTPIN_A4988_STEP);

  if (stepper)
  {
    stepper->setDirectionPin(OUTPIN_A4988_DIR);
    stepper->setEnablePin(OUTPIN_A4988_EN);
    stepper->setAutoEnable(true);

    // If auto enable/disable need delays, just add (one or both):
    // stepper->setDelayToEnable(50);
    // stepper->setDelayToDisable(1000);

    stepper->setSpeedInUs(2000); // the parameter is us/step !!!
    stepper->setAcceleration(200);
    // stepper->move(10000);
  }

  Start_Timer(TIME_INTERRUPT);

  // CONNECTION_WiFi(ATTEMPTS);
  BLYNK_connection(ATTEMPTS);

  Programming_OTA();

  TimeCheck = TIME_CHECK_CONNECTION;

  terminal.println("Start!");
  terminal.flush();
}

void loop()
{

  // faz o dispositivo ficar desconectando
  //  if (MyESP32.CheckConnection)
  //  {
  //    CONNECTION_reconnect(ATTEMPTS);
  //    BLYNK_reconnect(ATTEMPTS);
  //    MyESP32.CheckConnection = false;
  //  }

  Blynk.run();

  ArduinoOTA.handle();

  // delay(1500);

  /* deep_sleep mode */
  // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  // esp_deep_sleep_start();

  /* light sleep mode*/
  // esp_sleep_enable_timer_wakeup(1000000); // 1 second
  // int ret = esp_light_sleep_start();
  // Serial.printf("lp: %d\n", ret);

  if (stepper && (MyMoves.movRoboLeft || MyMoves.movRoboRight))
  {
    if (MyMoves.movRoboLeft)
    {
      stepper->setDirectionPin(OUTPIN_A4988_DIR, true);
      MyMoves.movRoboLeft = false;
    }
    else
    {
      stepper->setDirectionPin(OUTPIN_A4988_DIR, false);
      MyMoves.movRoboRight = false;
    }
    // stepper->setDirectionPin(OUTPIN_A4988_DIR);
    stepper->setEnablePin(OUTPIN_A4988_EN);
    stepper->setAutoEnable(true);

    // If auto enable/disable need delays, just add (one or both):
    // stepper->setDelayToEnable(50);
    // stepper->setDelayToDisable(1000);

    stepper->setSpeedInUs(100); // the parameter is us/step !!!
    stepper->setAcceleration(500);
    stepper->move(10000);
  }
}

// BLYNK_WRITE(UNLOCK_PORT)
// {
//   int value = param.asInt(); // Get value as integer
//   if (value)
//   {
//     digitalWrite(PIN_SOLENOIDE, HIGH);
//     delay(2000);
//     digitalWrite(PIN_SOLENOIDE, LOW);
//   }
// }

// BLYNK_READ(GET_VBAT)
// {
//   //   #define ADC_VBAT 13
//   // #define RES_R1 47000.0
//   // #define RES_R2 10000.0
//   // #define VREF 3.30
//   // #define RESOLUTION_ADC 4095
//   // Serial.println("*** Read request from Blynk ***");
//   unsigned int samples = SAMPLES;
//   float potValue = 0;

//   while (samples--)
//   {
//     potValue += analogRead(ADC_VBAT);
//   }

//   potValue = potValue / SAMPLES;
//   // Serial.println("Bateria RAW: " + (String)potValue);
//   potValue = (potValue * VREF) / RESOLUTION_ADC;
//   // Serial.println("Bateria RAW: " + (String)potValue);
//   potValue = potValue * ((RES_R1 + RES_R2) / RES_R2);
//   Serial.println("Bateria: " + (String)potValue + "V");
//   Blynk.virtualWrite(GET_VBAT, potValue);
// }

// BLYNK_WRITE(MOV_CAN_TILT)
// {
//   MyMoves.movCanTilt = true;
//   MyMoves.valueCanTilt = param.asInt();
// }

BLYNK_WRITE(MOV_ROBO_RIGHT)
{

  MyMoves.valueRoboRight = param.asInt();

  if (MyMoves.valueRoboRight)
  {
    terminal.println("MOV_ROBO_RIGH!");
    terminal.flush();
    MyMoves.movRoboRight = true;
  }
  // else
  // {
  //   MyMoves.movRoboRight = false;
  // }
}

BLYNK_WRITE(MOV_ROBO_LEFT)
{
  MyMoves.valueRoboLeft = param.asInt();

  if (MyMoves.valueRoboLeft)
  {
    terminal.println("MOV_ROBO_LEFT!");
    terminal.flush();
    MyMoves.movRoboLeft = true;
  }
  // else
  // {
  //   MyMoves.movRoboLeft = false;
  // }
}

BLYNK_WRITE(RESET_WIFI_V11)
{
  int value = param.asInt(); // Get value as integer
  Serial.println("Reset");

  terminal.println("Reset!");
  terminal.flush();

  if (value)
  {
    WiFiManager wifiConnect;
    wifiConnect.resetSettings();
    WiFi.disconnect();
    TimeCheck = 1;
    SPIFFS.format();
    ESP.restart();
  }
}
