#include "A9G_Bike_Locker.h"

#define MAIN_TASK_STACK_SIZE    (1024 * 2)
#define MAIN_TASK_PRIORITY      0
#define MAIN_TASK_NAME         "MAIN Test Task"

#define BUTTON_TASK_STACK_SIZE    (1024 * 2)
#define BUTTON_TASK_PRIORITY      1
#define BUTTON_TASK_NAME         "BUTTON Task"

#define POWER_MANAGEMENT_TASK_STACK_SIZE (1024 * 2)
#define POWER_MANAGEMENT_TASK_PRIORITY 2
#define POWER_MANAGEMENT_TASK_NAME "POWER MANAGEMENT Task"

static HANDLE mainTaskHandle = NULL;
static HANDLE signalLedTaskHandle = NULL;
static HANDLE powerLedTaskHandle = NULL;
static HANDLE fifthTaskHandle = NULL;

void buttonTask()
{
  while(1)
  {
    Trace(1,"Nwm co BUTTON");
    OS_Sleep(500);
    POWER_LED_DUTY_CYCLE++;
    if(POWER_LED_DUTY_CYCLE>20)
      POWER_LED_DUTY_CYCLE=0;//Sleep 500 ms
  }
}

void powerManagementTask()
{
  uint8_t percent;
  while(1)
  {
    uint16_t voltage = PM_Voltage(&percent);
    Trace(1,"voltage:%dmV,%d percent last,times count:%d",voltage,percent);
    OS_StartTask(signalLedTaskHandle,NULL);
    OS_Sleep(3000);
    OS_StopTask(signalLedTaskHandle);
    OS_Sleep(3000);                                             //Sleep 500 ms
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

    signalLedTaskHandle = OS_CreateTask(signalLedTask, NULL, NULL, SIGNAL_LED_TASK_STACK_SIZE,  SIGNAL_LED_TASK_PRIORITY,  OS_CREATE_SUSPENDED , 0, SIGNAL_LED_TASK_NAME);
    powerLedTaskHandle  = OS_CreateTask(powerLedTask,  NULL, NULL, POWER_LED_TASK_STACK_SIZE,   POWER_LED_TASK_PRIORITY,   OS_CREATE_SUSPENDED , 0, POWER_LED_TASK_NAME);
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
