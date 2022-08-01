// For the current project, we are just using a single Core. Dedicating the other core for future use. 
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define DELAY_500MS pdMS_TO_TICKS(500)
#define DELAY_10MS  pdMS_TO_TICKS(10)
// Global Variable

typedef struct Message {
  char body[100];
  int count;
} gUartMessage;




// Handles for Queues
QueueHandle_t serialWriteQueue;
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
    gUartMessage msg;
    strcpy(msg.body, serialMsg);
    msg.count = 1;
    xQueueSend( serialWriteQueue , (void *)&msg, portMAX_DELAY);
    
  
  
}






void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Creation of Serial Uart Queue for writing data to Serial Monitor
  serialWriteQueue = xQueueCreate(100 , sizeof(gUartMessage));
  
  
  
  // Creating intialization task
  xTaskCreatePinnedToCore (
    InitializationTask,
    "Initialization Task",
    1024 ,
    NULL,
    3,
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

   while(1)
  {
    writeQueue ("Initialization Task \r\n");
    vTaskDelay(DELAY_10MS);
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
        writeQueue ("input Task \r\n");
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
        writeQueue ("Output Task \r\n");

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
        writeQueue ("MQTT Task \r\n");
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
        writeQueue ("OLED Display Task \r\n");
    vTaskDelay(DELAY_10MS);
    
  }
}
 
/*
 * This is a Serial Uart Task
 */

void SerialUartTask(void *pvParam)
{

  gUartMessage receiveMsg;
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
