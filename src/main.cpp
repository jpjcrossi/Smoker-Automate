#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <NetworkService.h>
#include <MessageService.h>
#include <PWM.h>
#include "users-variables.h"
#include <PID.h>
#include <WiFi.h>
#include "./Models/OffSetModel.h"

// Core number definitions
static uint8_t taskCoreZero = 0;
static uint8_t taskCoreOne = 1;
NetworkService networkService(WiFi);
MessageService messageService(broker, port, mqttuser, mqttpass);
PID pid(1.0, 0, 0);
PWM pwm(16, 17, 21, 22);
OffSetModel OffSet;

void ReadTemperature(void *pvParameters)
{
  while (true)
  {
    /*  if(network.IsConnected())
    {
      Serial.print("Wifi Conectado.");
    }
    else
    {
      Serial.print("Wifi Desconectado.");
    }

     */

    delay(1000);
  }
}

void PIDController(void *pvParameters)
{
  double PIDResult = 0;
  int vpwm = 0;
  while (true)
  {
    pid.setSetPoint(OffSet.OffSet);
    pid.addNewSample(OffSet.Temperature);
    PIDResult = pid.process();
    //Serial.println(PIDResult);
    //Serial.println(OffSet.Temperature);

    Serial.println("------------------------------------------------------------------------------------");
    Serial.print("OffSet:");
    Serial.println(OffSet.OffSet);

    Serial.print("Temperature:");
    Serial.println(OffSet.Temperature);

    Serial.print("PID:");
    Serial.println(PIDResult);
    if (PIDResult > -4 &&  PIDResult < 4)
    {
      vpwm = 40;
    }
    else if (PIDResult < 0)
    {
      vpwm = 0;
    }
    else
    {
      vpwm = PIDResult + 50;
    }

    Serial.print("PWM:");
    Serial.println(vpwm);

    pwm.SetDutyCycle(vpwm);

    delay(5000);
  }
}

void MessagesHandle(void *OffSetModel)
{
  while (true)
  {
    messageService.Loop();
  }
}

void StartStopAction(void *pvParameters)
{
  while (true)
  {
  }
}

void getMessage(OffSetModel _OffSet)
{
  OffSet = _OffSet;
  //Serial.println(_OffSet.OffSet);
  //Serial.println(_OffSet.Temperature);
}

void setup()
{
  Serial.begin(115200);
  networkService.Connect(ssid, password);
  messageService.callbackMessage = getMessage;
  messageService.Connect(subscribeTopic, alertTopic, operationTopic);
  pwm.SetDutyCycle(50);

  // xTaskCreatePinnedToCore(
  //     ReadTemperature,   /* Function  */
  //     "readTemperature", /* Task name */
  //     10000,             /* Number of words to be staked */
  //     NULL,              /* Parameters (it could be NULL) */
  //     1,                 /* Priority task number (0 a N) */
  //     NULL,              /* task reference (it could be NULL) */
  //     taskCoreZero);
  // delay(500); //Just give some time before the task starts

  xTaskCreatePinnedToCore(
      PIDController,     /* Function  */
      "readTemperature", /* Task name */
      10000,             /* Number of words to be staked */
      NULL,              /* Parameters (it could be NULL) */
      2,                 /* Priority task number (0 a N) */
      NULL,              /* task reference (it could be NULL) */
      taskCoreZero);
  delay(500); //Just give some time before the task starts

  xTaskCreatePinnedToCore(
      MessagesHandle,    /* Function  */
      "readTemperature", /* Task name */
      10000,             /* Number of words to be staked */
      NULL,              /* Parameters (it could be NULL) */
      3,                 /* Priority task number (0 a N) */
      NULL,              /* task reference (it could be NULL) */
      taskCoreOne);
  delay(500); //Just give some time before the task starts

  // xTaskCreatePinnedToCore(
  //     StartStopAction,   /* Function  */
  //     "readTemperature", /* Task name */
  //     10000,             /* Number of words to be staked */
  //     NULL,              /* Parameters (it could be NULL) */
  //     1,                 /* Priority task number (0 a N) */
  //     NULL,              /* task reference (it could be NULL) */
  //     taskCoreOne);
  // delay(500); //Just give some time before the task starts
}

void loop()
{
}
