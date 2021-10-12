#include <WiFi.h>
#include <max6675.h>
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <freertos/FreeRTOS.h>

#include <Headers/PID.h>
#include <Headers/PWM.h>
#include <Headers/MessageService.h>
#include <Headers/NetworkService.h>

#include <users-variables.h>

#include <Models/OffSetModel.h>
#include <Models/FeedBackModel.h>

// Core number definitions
static uint8_t taskCoreZero = 0;
//static uint8_t taskCoreOne = 1;
NetworkService networkService(WiFi);
MessageService messageService(broker, port, mqttuser, mqttpass);
MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);

PID pid(kP, kI, kD);
PWM pwm(FanPin1, FanPin2, FanenablePin);

OffSetModel OffSet;
FeedBackModel feedBack;

void ReadTemperature(void *pvParameters)
{
  while (true)
  {
    delay(2000);
    feedBack.Temperature_Value = (int)(thermocouple.readCelsius() + TemperatureOffSet);
    Serial.println("------------------------------------------------------------------------------------");
    Serial.print("Temperature_Value:");
    Serial.println(feedBack.Temperature_Value);

    messageService.SendFeedBack(feedBack);
  }
}

void MinutDelay(int minuts)
{
  unsigned long Millis = minuts * 60000;
  while (Millis > 0)
  {
    delay(1000);
    Millis -= 1000;
  }
}

void PIDController(void *pvParameters)
{
  double PIDResult = 0;
  int vpwm = 0;
  double Tolerance = 0;
  while (true)
  {
    Tolerance = OffSet.Tolerance / 100;

    pid.setSetPoint(OffSet.OffSet);
    pid.addNewSample(feedBack.Temperature_Value);
    PIDResult = pid.process();

    Serial.println("------------------------------------------------------------------------------------");
    Serial.print("OffSet:");
    Serial.println(OffSet.OffSet);

    Serial.print("Temperature:");
    Serial.println(feedBack.Temperature_Value);

    Serial.print("PID:");
    Serial.println(PIDResult);

    Serial.print("NaturalFlow:");
    Serial.println(OffSet.NaturalFlow);

    Serial.print("Tolerance:");
    Serial.println(Tolerance);
/*
    if (PIDResult * (1 - Tolerance) > -4 && PIDResult * (1 + Tolerance) < 4)
    {
      vpwm = OffSet.NaturalFlow;
    }
    else if (PIDResult < 0)
    {
      vpwm = 0;
    }
    else
    {
      vpwm = PIDResult + OffSet.NaturalFlow;
    }
*/
    feedBack.PWM_Value = vpwm;
    feedBack.Fan_Value = PIDResult;

    Serial.print("PWM:");
    Serial.println(vpwm);

    pwm.SetDutyCycle(vpwm);
    delay(500);
  }
}

void MessagesHandle(void *OffSetModel)
{
  while (true)
  {
    messageService.Loop();
  }
}

void getMessage(OffSetModel _OffSet)
{
  OffSet = _OffSet;
  Serial.println("------------------------------------------------------------------------------------");
  Serial.print("OffSet:");
  Serial.println(OffSet.OffSet);

  Serial.print("NaturalFlow:");
  Serial.println(OffSet.NaturalFlow);

  Serial.print("Tolerance:");
  Serial.println(OffSet.Tolerance);
  Serial.println("------------------------------------------------------------------------------------");  
}

void setup()
{
  Serial.begin(115200);
  
  networkService.Connect(ssid, password);
  messageService.callbackMessage = getMessage;  
  messageService.Connect(subscribeTopic, alertTopic, operationTopic);

  pwm.SetDutyCycle(0);

  ArduinoOTA.setHostname("Smoker_Automate");
  ArduinoOTA.onStart([]()
                     {
                       String type;
                       if (ArduinoOTA.getCommand() == U_FLASH)
                         type = "sketch";
                       else // U_SPIFFS
                         type = "filesystem";

                       // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                       Serial.println("Start updating " + type);
                     });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
                       Serial.printf("Error[%u]: ", error);
                       if (error == OTA_AUTH_ERROR)
                         Serial.println("Auth Failed");
                       else if (error == OTA_BEGIN_ERROR)
                         Serial.println("Begin Failed");
                       else if (error == OTA_CONNECT_ERROR)
                         Serial.println("Connect Failed");
                       else if (error == OTA_RECEIVE_ERROR)
                         Serial.println("Receive Failed");
                       else if (error == OTA_END_ERROR)
                         Serial.println("End Failed");
                     });
  ArduinoOTA.begin();

  xTaskCreatePinnedToCore(
      ReadTemperature,   /* Function  */
      "readTemperature", /* Task name */
      10000,             /* Number of words to be staked */
      NULL,              /* Parameters (it could be NULL) */
      1,                 /* Priority task number (0 a N) */
      NULL,              /* task reference (it could be NULL) */
      taskCoreZero);
  delay(500); //Just give some time before the task starts

   xTaskCreatePinnedToCore(
       PIDController,     /* Function  */
       "readTemperature", /* Task name */
       10000,             /* Number of words to be staked */
       NULL,              /* Parameters (it could be NULL) */
       2,                 /* Priority task number (0 a N) */
       NULL,              /* task reference (it could be NULL) */
       taskCoreZero);
   delay(500); //Just give some time before the task starts
}

void loop()
{
  ArduinoOTA.handle();
  messageService.Loop();
 // pwm.SetDutyCycle(100);
}
