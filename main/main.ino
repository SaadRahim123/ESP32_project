// For the current project, we are just using a single Core. Dedicating the other core for future use. 
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif



/*
 *  Includes 
 *  
 */

#include "stdlib.h"

#include <PubSubClient.h>
#include <Preferences.h>
#include <string.h>
#include "settings.hpp"
#include "oled.hpp"
#include "outputs.hpp"
#include "inputs.hpp"
#include "timers.hpp"
#include "buttons.hpp"
#include "wifi.hpp"
#include "time.hpp"
#include "watchdog.hpp"
#include "defs.hpp"
#include "sdcard.hpp"
#include "memory.hpp"


/*
 * Defines
 */
 
#define DELAY_500MS pdMS_TO_TICKS(500)
#define DELAY_10MS  pdMS_TO_TICKS(10)

// Define Externs
_Timer Timer;
_Input Input;
_Output Output;
_Wifi Wifi;
_Time Time;
_Nvs Nvs;

Buttons buttons;
Oled Oled;
WDT Wdt;
Sdcard Sdcard;


WiFiClient espClient;
PubSubClient client(espClient);
Preferences preferences;


// Global Variable
char *inputMessage = "";
char *payloadAsChar = "";
bool publishInputMessage = false;

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;
const char *mqtt_ID = "esp32a01";
// MQTT Credentials
const char *mqtt_username = "remote2";
const char *mqtt_password = "password2";
const char *mqtt_client = "ESP32A01";

int reconnectCounter = 0;
long lastReconnectAttempt = 0;

long lastUpdate = 0;
int counter = 1;
long lastTachoTime = 0;
int rpm;

bool inputBool = false;

using namespace std;

typedef struct Message {
  char body[100];
  int count;
} gGeneralMessage;




// Handles for Queues
QueueHandle_t serialWriteQueue;
QueueHandle_t initializationQueue;


// Tasks Priority

// Tasks prototypes
void InitializationTask (void *pvParam);
void InputTask (void *pvParam);
void OutputTask (void *pvParam);
void SerialUartTask (void *pvParam);
void MQTT_Task (void *pvParam);
void OLED_DisplayTask(void *pvParam);
// Handles for Tasks
TaskHandle_t Initialization_Task_Handler , Input_Task_Handler, Output_Task_Handler, Display_Task_Handler, Serial_Task_Handler;
TaskHandle_t MQTT_Task_Handler;






// Global Functions   portMAX_DELAY
void writeQueue(char serialMsg[])
{
    gGeneralMessage msg;
    strcpy(msg.body, serialMsg);
    msg.count = 1;
    xQueueSend( serialWriteQueue , (void *)&msg, portMAX_DELAY);
}






void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Creation of Serial Uart Queue for writing data to Serial Monitor
  serialWriteQueue = xQueueCreate(100 , sizeof(gGeneralMessage));

     // Nvs.begin();                                   // Update settings from nvs memory
     Serial.begin(115200);
     Wire.begin(SDA_PIN, SCL_PIN);  // this should be after Sensors.begin()
     Serial.print("Running software V");
     Serial.println(settings["version"]);
     Oled.begin();
     Oled.displayln("Starting....");
     delay(500);

    // setup MQTT broker
 //   client.setServer(mqtt_broker, mqtt_port);
  //  client.setCallback(callback);
 // 
  //  buttons.begin();
  //  Output.begin();
   // Input.begin();
    Wifi.connect();
   // Wdt.begin();
    //Sdcard.begin();
  
  
    // Connect to MQTT broker
 //  reconnect();
 
  
  
  // Creating intialization task
  xTaskCreatePinnedToCore (
    InitializationTask,
    "Initialization Task",
    1024 ,
    NULL,
    4,
    &Initialization_Task_Handler,
    app_cpu );

   // Creating Input Task
   xTaskCreatePinnedToCore (InputTask,    "Input Task",    1024 ,    NULL,    3,    &Input_Task_Handler,    app_cpu );
  // Creating Output Task
   xTaskCreatePinnedToCore (OutputTask,    "Output Task",    1024 ,    NULL,   3,    &Output_Task_Handler,    app_cpu );
   // Creating Serial Uart Task 
   xTaskCreatePinnedToCore (SerialUartTask,    "Serial UART Task",    1024 ,    NULL,   3,    &Serial_Task_Handler,    app_cpu );
   // Creating MQTT Task
   xTaskCreatePinnedToCore (MQTT_Task,    "MQTT Task",   4096  ,    NULL,   3,    &MQTT_Task_Handler,    app_cpu );
   // Creating Display Task
   xTaskCreatePinnedToCore (OLED_DisplayTask,    "Display Oled Task",   1024  ,    NULL,   3,    &Display_Task_Handler,    app_cpu );
 
  
}

/*
 *  This is an Initialization Task
 */
void InitializationTask(void *pvParam)
{
  gGeneralMessage receiveMsg;
   while(1)
  {
    
    // making the task to go into the blocking mode. This functionality
    // is added for future purposes
    if ( xQueueReceive(initializationQueue, (void*)&receiveMsg , portMAX_DELAY) == pdTRUE) 
    {
       // Wait Here until a meessage is sent to the Queue.
    }

    // #TODO: Suspend the task here 
  }
}

/*
 * This is an Input Task. This task is responsible
 * for all the input handling of the system
 */
void InputTask (void *pvParam)
{
  while(1)
  {
        //writeQueue ("input Task \r\n");
        vTaskDelay(DELAY_10MS);
  }
}

/*
 * This is an output Task. This task is responsible 
 * for handling all the outputs of the system
 */
void OutputTask (void *pvParam)
{
  while(1)
  {
        //writeQueue ("Output Task \r\n");

        vTaskDelay(DELAY_10MS);
  }
}

/*
 * 
 */
void MQTT_Task (void *pvParam)
{
  while(1)
  {
       // writeQueue ("MQTT Task \r\n");
    vTaskDelay(DELAY_10MS);
    
  }
}

/*
 * 
 */
void OLED_DisplayTask(void *pvParam)
{
  while(1)
  {
      //  writeQueue ("OLED Display Task \r\n");
    vTaskDelay(DELAY_10MS);
    
  }
}
 
/*
 * This is a Serial Uart Task
 */

void SerialUartTask(void *pvParam)
{

  gGeneralMessage receiveMsg;
  while(1)
  {
      if ( xQueueReceive(serialWriteQueue, (void*)&receiveMsg , portMAX_DELAY) == pdTRUE) 
      {
        Serial.print(receiveMsg.body);
        Serial.println(receiveMsg.count);
        Serial.flush();
      }

  }
 
}



void loop() {
  // put your main code here, to run repeatedly:

}
