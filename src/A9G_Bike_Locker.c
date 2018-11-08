#include "A9G_Bike_Locker.h"

#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME         "MAIN Test Task"

#define BUTTON_TASK_STACK_SIZE    (1024 * 2)
#define BUTTON_TASK_PRIORITY      1
#define BUTTON_TASK_NAME         "BUTTON Task"

#define LED_TASK_STACK_SIZE    (1024 * 2)
#define LED_TASK_PRIORITY      1
#define LED_TASK_NAME         "LED Task"

#define POWER_MANAGEMENT_TASK_STACK_SIZE (1024 * 2)
#define POWER_MANAGEMENT_TASK_PRIORITY 2
#define POWER_MANAGEMENT_TASK_NAME "POWER MANAGEMENT Task"


static HANDLE mainTaskHandle = NULL;
static HANDLE secondTaskHandle = NULL;
static HANDLE thirtTaskHandle = NULL;
static HANDLE fifthTaskHandle = NULL;



void ledTask()
{

  GPIO_Init(gpioLedBlue);
  GPIO_Init(gpioLedGreen);

  GPIO_SetLevel(gpioLedBlue,ledBlueLevel); //Set level
  GPIO_SetLevel(gpioLedGreen,ledGreenLevel); //Set level

  while(1)
  {
    ledBlueLevel   = (ledBlueLevel==GPIO_LEVEL_HIGH)?GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH;
    ledGreenLevel  = (ledGreenLevel==GPIO_LEVEL_HIGH)?GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH;
    GPIO_SetLevel(gpioLedBlue,ledBlueLevel);
    GPIO_SetLevel(gpioLedGreen,ledGreenLevel);
    OS_Sleep(blue_led_pwm_high_intensity);
    ledBlueLevel   = (ledBlueLevel==GPIO_LEVEL_HIGH)?GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH;
    ledGreenLevel  = (ledGreenLevel==GPIO_LEVEL_HIGH)?GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH;
    GPIO_SetLevel(gpioLedBlue,ledBlueLevel);
    GPIO_SetLevel(gpioLedGreen,ledGreenLevel);

    OS_Sleep(pwm_led_period-blue_led_pwm_high_intensity);
  }
}

void buttonTask()
{
  while(1)
  {
    Trace(1,"Nwm co BUTTON");
    OS_Sleep(500);
    blue_led_pwm_high_intensity++;
    if(blue_led_pwm_high_intensity>20)
      blue_led_pwm_high_intensity=0;//Sleep 500 ms
  }
}

void powerManagementTask()
{
  uint8_t percent;
  while(1)
  {
    uint16_t voltage = PM_Voltage(&percent);
    Trace(1,"voltage:%dmV,%d percent last,times count:%d",voltage,percent);
    OS_Sleep(500);                                  //Sleep 500 ms
  }
}

void EventDispatch(API_Event_t* pEvent)
{
    switch(pEvent->id)
    {
        default:
            break;
    }
}


void MainTask(void *pData)
{
    API_Event_t* event=NULL;

    secondTaskHandle = OS_CreateTask(buttonTask,NULL, NULL, BUTTON_TASK_STACK_SIZE, BUTTON_TASK_PRIORITY, 0, 0, BUTTON_TASK_NAME);
    thirtTaskHandle  = OS_CreateTask(ledTask,NULL, NULL, LED_TASK_STACK_SIZE, LED_TASK_PRIORITY, 0, 0, LED_TASK_NAME);
    fifthTaskHandle  = OS_CreateTask(powerManagementTask,NULL, NULL, POWER_MANAGEMENT_TASK_STACK_SIZE, POWER_MANAGEMENT_TASK_PRIORITY, 0, 0, POWER_MANAGEMENT_TASK_NAME);

    while(1)
    {
        if(OS_WaitEvent(mainTaskHandle, (void**)&event, OS_TIME_OUT_WAIT_FOREVER))
        {
            EventDispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

void A9G_Bike_Locker_Main()
{
    mainTaskHandle = OS_CreateTask(MainTask , NULL, NULL, MAIN_TASK_STACK_SIZE, MAIN_TASK_PRIORITY, 0, 0, MAIN_TASK_NAME);
    OS_SetUserMainHandle(&mainTaskHandle);
}
