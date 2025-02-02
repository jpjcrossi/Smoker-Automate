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
static uint8_t taskCoreOne = 1;
NetworkService networkService(WiFi);
MessageService messageService(broker, port, mqttuser, mqttpass);
MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);

PID pid(kP, kI, kD);
PWM pwm(FanPin1, FanPin2, FanenablePin);

OffSetModel OffSet;
FeedBackModel feedBack;

// Enum para estados do smoker


SmokerState smokerState = SmokerState::RELEASED_TO_FIX; // Variável global para armazenar o estado

// Função para leitura de temperatura
void ReadTemperature(void *pvParameters)
{
    int Temperature = 0;
    int lastSentTemperature = -1; // Armazena o último valor de temperatura enviado
    int countInt = 0;

    TickType_t xLastWakeTime = xTaskGetTickCount(); // Obtém o tempo atual

    while (true)
    {
        // Atualiza a força do sinal Wi-Fi a cada 60 segundos (30 iterações * 2 segundos)
        if (countInt == 0)
        {
            int32_t rssi = WiFi.RSSI();
            feedBack.WiFiSignalStrength = map(constrain(rssi, -100, -50), -100, -50, 0, 100);
        }

        // Lê a temperatura do termopar
        Temperature = (int)(thermocouple.readCelsius() + TemperatureOffSet);

        // Verifica se a temperatura é válida e se houve mudança
        if (Temperature <= 200 && Temperature != lastSentTemperature)
        {
            feedBack.Temperature_Value = Temperature;
            lastSentTemperature = Temperature; // Atualiza o último valor enviado

            Serial.println("------------------------------------------------------------------------------------");
            Serial.print("Temperature_Value: ");
            Serial.println(feedBack.Temperature_Value);

            // Atualiza o estado do smoker
            switch (smokerState)
            {
                case SmokerState::RELEASED_TO_FIX:
                    feedBack.State_Value = "Pronto para corrigir";
                    break;
                case SmokerState::APPLYING_CORRECTION:
                    feedBack.State_Value = "Aplicando Correção";
                    break;
                case SmokerState::STABILIZING:
                    feedBack.State_Value = "Aguardando Estabilizar";
                    break;
            }

            // Envia o feedback apenas se a temperatura mudou
            messageService.SendFeedBack(feedBack);
        }

        // Incrementa o contador e reinicia após 30 iterações (60 segundos)
        countInt = (countInt + 1) % 30;

        // Aguarda até o próximo ciclo de 2 segundos
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000)); // 2000 ms = 2 segundos
    }
}

// Função para controle de estado
void StateController(void *pvParameters)
{
    int interationCount = 0;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true)
    {
        if (smokerState == SmokerState::RELEASED_TO_FIX || (OffSet.TimeToCorrect + OffSet.TimeToStabilize == 0))
        {
            interationCount = 0;
        }
        else if (smokerState == SmokerState::APPLYING_CORRECTION)
        {
            interationCount++;
            if (interationCount >= OffSet.TimeToCorrect)
            {
                interationCount = 0;
                smokerState = SmokerState::STABILIZING;
            }
        }
        else if (smokerState == SmokerState::STABILIZING)
        {
            interationCount++;
            if (interationCount >= OffSet.TimeToStabilize)
            {
                interationCount = 0;
                smokerState = SmokerState::RELEASED_TO_FIX;
            }
        }

        // Aguarda até o próximo ciclo de 1 segundo
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000)); // 1000 ms = 1 segundo
    }
}

// Função para controle PID
void PIDController(void *pvParameters)
{
    double PIDResult = 0;
    int PWM = 0;
    double Tolerance = 0;
    double Variance = 0;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true)
    {
        // Verifica se o estado não é APPLYING_CORRECTION e se a temperatura é válida
        if (smokerState != SmokerState::APPLYING_CORRECTION && feedBack.Temperature_Value >= OffSet.MinValue && OffSet.MinValue != 0)
        {
            Tolerance = OffSet.Tolerance;

            pid.setSetPoint(OffSet.OffSet);
            pid.addNewSample(feedBack.Temperature_Value);
            PIDResult = pid.process();

            if (PIDResult > 0)
            {
                Variance = OffSet.OffSet - feedBack.Temperature_Value;
                if (Variance > Tolerance)
                {
                    PWM = FanOffSet + PIDResult;
                    smokerState = SmokerState::APPLYING_CORRECTION; // Atualiza o estado
                }
                else
                {
                    PWM = 0;
                }
            }
            else
            {
                PWM = 0;
            }

            if (PWM > 100)
                PWM = 100;

            feedBack.PWM_Value = PWM;
        }
        else
        {
            feedBack.PWM_Value = 0;
        }

        // Aguarda até o próximo ciclo de 500 ms
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(500)); // 500 ms = 0.5 segundos
    }
}

// Função para lidar com mensagens MQTT
void MessagesHandle(void *pvParameters)
{
    while (true)
    {
        messageService.Loop();
    }
}

// Callback para receber mensagens MQTT
void getMessage(OffSetModel _OffSet)
{
    OffSet = _OffSet;
}

// Setup
void setup()
{
    Serial.begin(115200);

    networkService.Connect(ssid, password);
    messageService.callbackMessage = getMessage;
    messageService.Connect(subscribeTopic, alertTopic, operationTopic);

    pwm.SetDutyCycle(0);

    ArduinoOTA.setHostname("Smoker_Automate");
    ArduinoOTA.begin();

    xTaskCreatePinnedToCore(
        ReadTemperature,   /* Function  */
        "readTemperature", /* Task name */
        10000,             /* Stack size */
        NULL,              /* Parameters */
        0,                 /* Priority */
        NULL,              /* Task handle */
        taskCoreZero);

    xTaskCreatePinnedToCore(
        PIDController,   /* Function  */
        "PIDController", /* Task name */
        10000,           /* Stack size */
        NULL,            /* Parameters */
        2,               /* Priority */
        NULL,            /* Task handle */
        taskCoreZero);

    xTaskCreatePinnedToCore(
        StateController,  /* Function  */
        "StateController", /* Task name */
        10000,            /* Stack size */
        NULL,             /* Parameters */
        1,                /* Priority */
        NULL,             /* Task handle */
        taskCoreOne);
}

// Loop principal
void loop()
{
    ArduinoOTA.handle();
    messageService.Loop();

    static unsigned long startTime = millis();
    if (millis() - startTime < 2000)
    {
        pwm.SetDutyCycle(100);
    }
    else
    {
        pwm.SetDutyCycle(feedBack.PWM_Value);
    }
}