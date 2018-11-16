#include "api_hal_gpio.h"
#include "api_debug.h"
#include "api_os.h"
#include "api_hal_pm.h"
#include "api_os.h"
#include "api_event.h"

#include "stdint.h"
#include "stdbool.h"

#ifndef __PWM_LED_H_
#define __PWM_LED_H_

#define POWER_LED_TASK_STACK_SIZE    (1024 * 2)
#define POWER_LED_TASK_PRIORITY      1
#define POWER_LED_TASK_NAME          "POWER_LED Task"

#define SIGNAL_LED_TASK_STACK_SIZE    (1024 * 2)
#define SIGNAL_LED_TASK_PRIORITY      1
#define SIGNAL_LED_TASK_NAME          "LED Task"

#define POWER_LED   GPIO_PIN27
#define SIGNAL_LED  GPIO_PIN28

uint8_t LED_PERIOD = 20;
uint8_t POWER_LED_DUTY_CYCLE  =0;
uint8_t SIGNAL_LED_DUTY_CYCLE =0;

static GPIO_LEVEL power_led_level  = GPIO_LEVEL_LOW;
static GPIO_LEVEL signal_led_level = GPIO_LEVEL_HIGH;

GPIO_config_t power_led = {
    .mode         = GPIO_MODE_OUTPUT,
    .pin          = POWER_LED,
    .defaultLevel = GPIO_LEVEL_LOW
};

GPIO_config_t signal_led = {
    .mode         = GPIO_MODE_OUTPUT,
    .pin          = SIGNAL_LED,
    .defaultLevel = GPIO_LEVEL_HIGH
};

GPIO_LEVEL toogle(GPIO_LEVEL led_level)
{
  led_level = (led_level==GPIO_LEVEL_HIGH)?GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH;
  return led_level;
}

void powerLedTask()
{
  GPIO_Init(power_led);
  GPIO_SetLevel(power_led, GPIO_LEVEL_LOW); //Set level
  while(1)
  {
    GPIO_SetLevel(power_led,toogle(power_led_level));
    OS_Sleep(POWER_LED_DUTY_CYCLE);
    GPIO_SetLevel(power_led,toogle(power_led_level));
    OS_Sleep(LED_PERIOD-POWER_LED_DUTY_CYCLE);
  }
  GPIO_SetLevel(power_led, GPIO_LEVEL_LOW); //Set level
}

void signalLedTask()
{
  GPIO_Init(signal_led);
  GPIO_SetLevel(signal_led, GPIO_LEVEL_LOW); //Set level
  while(1)
  {
    GPIO_SetLevel(signal_led,toogle(signal_led_level));
    OS_Sleep(SIGNAL_LED_DUTY_CYCLE);
    GPIO_SetLevel(signal_led,toogle(signal_led_level));
    OS_Sleep(LED_PERIOD-SIGNAL_LED_DUTY_CYCLE);
  }
  GPIO_SetLevel(signal_led, GPIO_LEVEL_LOW); //Set level
}

#endif
