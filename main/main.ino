// For the current project, we are just using a single Core. Dedicating the other core for future use. 
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define DELAY_500MS pdMS_TO_TICKS(500)

// Global Variable






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
void writeQueue(char msg[])
{
  char msgSerial[1024];
  // Write the data in the Queue
  sprintf(msgSerial, msg);
  if( (xQueueSendToBack( serialWriteQueue, &msgSerial , portMAX_DELAY)) == pdPASS )
  {
    
  }
  
  
}






void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Creation of Serial Uart Queue for writing data to Serial Monitor
  serialWriteQueue = xQueueCreate(100 , sizeof(char*));
  
  
  
  // Creating intialization task
  xTaskCreatePinnedToCore (
    InitializationTask,
    "Initialization Task",
    512 ,
    NULL,
    3,
    &Initialization_Task_Handler,
    app_cpu );

   // Creating Input Task
//   xTaskCreatePinnedToCore (InputTask,    "Input Task",    512 ,    NULL,    3,    &Input_Task_Handler,    app_cpu );
  // Creating Output Task
//   xTaskCreatePinnedToCore (OutputTask,    "Output Task",    512 ,    NULL,   3,    &Output_Task_Handler,    app_cpu );
   // Creating Serial Uart Task 
   xTaskCreatePinnedToCore (SerialUartTask,    "Serial UART Task",    512 ,    NULL,   3,    &Serial_Task_Handler,    app_cpu );
   // Creating MQTT Task
//   xTaskCreatePinnedToCore (MQTT_Task,    "MQTT Task",   4096  ,    NULL,   3,    &MQTT_Task_Handler,    app_cpu );
   // Creating Display Task
//   xTaskCreatePinnedToCore (OLED_DisplayTask,    "Display Oled Task",   4096  ,    NULL,   3,    &Display_Task_Handler,    app_cpu );
 
  
}

void InitializationTask(void *pvParam)
{
  while(1)
  {
    char temp[] = "Initialization Task \r\n";
    if( (xQueueSendToBack( serialWriteQueue, temp , portMAX_DELAY)) == pdPASS )
    {
      
    }
    vTaskDelay(DELAY_500MS);
  }
}

void SerialUartTask(void *pvParam)
{

  char *receiveMsg;
  while(1)
  {
      xQueueReceive(serialWriteQueue, (void*)&receiveMsg , portMAX_DELAY);
      Serial.println(receiveMsg);

  }
 
}




void loop() {
  // put your main code here, to run repeatedly:

}
