#include "api_hal_gpio.h"
#include "api_debug.h"
#include "api_os.h"
#include "api_hal_pm.h"
#include "api_os.h"
#include "api_event.h"

#include "stdint.h"
#include "stdbool.h"

#ifndef __BIKE_LOCKER_H_
#define __BIKE_LOCKER_H_

#define GPIO_PIN_LED_BLUE   GPIO_PIN27
#define GPIO_PIN_LED_GREEN  GPIO_PIN28

static GPIO_LEVEL ledBlueLevel  = GPIO_LEVEL_LOW;
static GPIO_LEVEL ledGreenLevel = GPIO_LEVEL_HIGH;

uint8_t blue_led_pwm_high_intensity = 0;
uint8_t pwm_led_period = 20;

GPIO_config_t gpioLedBlue = {
    .mode         = GPIO_MODE_OUTPUT,
    .pin          = GPIO_PIN27,
    .defaultLevel = GPIO_LEVEL_LOW
};

GPIO_config_t gpioLedGreen = {
    .mode         = GPIO_MODE_OUTPUT,
    .pin          = GPIO_PIN28,
    .defaultLevel = GPIO_LEVEL_HIGH
};

#endif
