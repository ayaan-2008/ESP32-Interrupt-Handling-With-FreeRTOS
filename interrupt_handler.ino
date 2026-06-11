#include <Arduino.h>

// ==========================
// Event Structure
// ==========================
struct InterruptEvent
{
    int source;
    int count;
};

// ==========================
// Pins
// ==========================
const int LED_PIN = 2;      // Blinking LED

const int INT_LED1 = 4;     // Interrupt 1 LED
const int INT_LED2 = 5;     // Interrupt 2 LED

const int BUTTON1 = 18;     // Interrupt Source 1
const int BUTTON2 = 19;     // Interrupt Source 2

// ==========================
// Globals
// ==========================
QueueHandle_t interruptQueue;

volatile int count1 = 0;
volatile int count2 = 0;

// ==========================
// ISR 1
// ==========================
void IRAM_ATTR button1ISR()
{
    count1++;

    InterruptEvent evt;

    evt.source = 1;
    evt.count = count1;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(
        interruptQueue,
        &evt,
        &xHigherPriorityTaskWoken
    );

    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}

// ==========================
// ISR 2
// ==========================
void IRAM_ATTR button2ISR()
{
    count2++;

    InterruptEvent evt;

    evt.source = 2;
    evt.count = count2;

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(
        interruptQueue,
        &evt,
        &xHigherPriorityTaskWoken
    );

    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}

// ==========================
// Task 1 - Blink LED
// ==========================
void ledTask(void *pvParameters)
{
    while (1)
    {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED ON");

        vTaskDelay(pdMS_TO_TICKS(500));

        digitalWrite(LED_PIN, LOW);
        Serial.println("LED OFF");

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// ==========================
// Task 2 - Sensor Simulation
// ==========================
void sensorTask(void *pvParameters)
{
    while (1)
    {
        int fakeSensor = random(0, 100);

        Serial.print("Sensor Value: ");
        Serial.println(fakeSensor);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// ==========================
// Task 3 - Interrupt Handler
// ==========================
void interruptTask(void *pvParameters)
{
    InterruptEvent evt;

    while (1)
    {
        if (xQueueReceive(
                interruptQueue,
                &evt,
                portMAX_DELAY))
        {
            if (evt.source == 1)
            {
                Serial.print("Interrupt 1 Received! Count = ");
                Serial.println(evt.count);

                digitalWrite(INT_LED1, HIGH);

                vTaskDelay(pdMS_TO_TICKS(500));

                digitalWrite(INT_LED1, LOW);
            }

            if (evt.source == 2)
            {
                Serial.print("Interrupt 2 Received! Count = ");
                Serial.println(evt.count);

                digitalWrite(INT_LED2, HIGH);

                vTaskDelay(pdMS_TO_TICKS(500));

                digitalWrite(INT_LED2, LOW);
            }
        }
    }
}

// ==========================
// Setup
// ==========================
void setup()
{
    Serial.begin(115200);

    // LEDs
    pinMode(LED_PIN, OUTPUT);
    pinMode(INT_LED1, OUTPUT);
    pinMode(INT_LED2, OUTPUT);

    // Buttons
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);

    // Queue
    interruptQueue = xQueueCreate(
        10,
        sizeof(InterruptEvent)
    );

    // Interrupt Attach
    attachInterrupt(
        digitalPinToInterrupt(BUTTON1),
        button1ISR,
        FALLING
    );

    attachInterrupt(
        digitalPinToInterrupt(BUTTON2),
        button2ISR,
        FALLING
    );

    // Create Tasks
    xTaskCreate(
        ledTask,
        "LED Task",
        2048,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        sensorTask,
        "Sensor Task",
        2048,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        interruptTask,
        "Interrupt Task",
        2048,
        NULL,
        2,
        NULL
    );

    Serial.println("RTOS Started");
}

void loop()
{
    // Empty
}
