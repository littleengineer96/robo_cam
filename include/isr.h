#ifndef ISR_H
#define ISR_H

#include "prototypes.h"

// extern unsigned long TimeCheck;
// extern unsigned long TimeOutConnect;
// extern unsigned long TimeOut;

// extern ESP_32 MyESP32;
// extern hw_timer_t *timer;

void My_Timer();

void Start_Timer(uint64_t time_in_us)
{
    /*
    https://www.dobitaobyte.com.br/isr-interrupcoes-e-timer-com-esp32/
    */

    // inicialização do timer. Parametros:
    /* 0 - seleção do timer a ser usado, de 0 a 3.
      80 - prescaler. O clock principal do ESP32 é 80MHz. Dividimos por 80 para ter 1us por tick.
    true - true para contador progressivo, false para regressivo
    */
    timer = timerBegin(0, 80, true);

    /*conecta à interrupção do timer
    - timer é a instância do hw_timer
    - &My_Timer endereço da função a ser chamada pelo timer
    - true gera uma interrupção
   */
    timerAttachInterrupt(timer, &My_Timer, true);

    /*
     - o timer instanciado no inicio
     - o valor em us para
     - auto-reload. true para repetir o alarme
    */
    timerAlarmWrite(timer, time_in_us, true);

    // ativa o alarme
    timerAlarmEnable(timer);
}

void My_Timer()
{
    CountTime += 1;

    // in offline mode
    if (MyESP32.OfflineMode)
    {
        digitalWrite(LED_BORD, bLedState);
        bLedState = !bLedState;
    }
    else
    {
        auxLed += 1;
        if (auxLed == 5)
        {
            digitalWrite(LED_BORD, bLedState);
            bLedState = !bLedState;
            auxLed = 0;
        }
    }

    // verifica a conexão wifi
    // if (TimeCheck)
    // {
    //     TimeCheck -= 1;

    //     if (TimeCheck == 0)
    //     {
    //         MyESP32.CheckConnection = true;
    //         TimeCheck = TIME_CHECK_CONNECTION;
    //     }
    // }
}
#endif