#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <Connect_To_NetWork.h>
#include "users-variables.h"
#include <WiFi.h>

// Core number definitions
static uint8_t taskCoreZero = 0;
static uint8_t taskCoreOne = 1;
Connect_To_NetWork network(ssid, password);

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
  while (true)
  {
  }
}

void MessagesHandle(void *pvParameters)
{
  while (true)
  {
  }
}

void StartStopAction(void *pvParameters)
{
  while (true)
  {
  }
}

void setup()
{
  Serial.begin(115200);

  network.Connect();

   xTaskCreatePinnedToCore(
       ReadTemperature,   /* Function  */
       "readTemperature", /* Task name */
       10000,             /* Number of words to be staked */
       NULL,              /* Parameters (it could be NULL) */
       1,                 /* Priority task number (0 a N) */
       NULL,              /* task reference (it could be NULL) */
       taskCoreZero);
   delay(500); //Just give some time before the task starts

  // xTaskCreatePinnedToCore(
  //     PIDController,     /* Function  */
  //     "readTemperature", /* Task name */
  //     10000,             /* Number of words to be staked */
  //     NULL,              /* Parameters (it could be NULL) */
  //     2,                 /* Priority task number (0 a N) */
  //     NULL,              /* task reference (it could be NULL) */
  //     taskCoreZero);
  // delay(500); //Just give some time before the task starts

  // xTaskCreatePinnedToCore(
  //     MessagesHandle,    /* Function  */
  //     "readTemperature", /* Task name */
  //     10000,             /* Number of words to be staked */
  //     NULL,              /* Parameters (it could be NULL) */
  //     3,                 /* Priority task number (0 a N) */
  //     NULL,              /* task reference (it could be NULL) */
  //     taskCoreOne);
  // delay(500); //Just give some time before the task starts

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



void loop() {}
