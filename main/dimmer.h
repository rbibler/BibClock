#include "utils.h"
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include <stddef.h>
#include "esp_intr_alloc.h"
#include "esp_attr.h"
#include "driver/ledc.h"


#define GCLK    GPIO_NUM_13
#define BLNK    GPIO_NUM_12
#define XLAT    GPIO_NUM_14
#define DSCLK   GPIO_NUM_27
#define SIN     GPIO_NUM_26

#define GCLK_CHANNEL  LEDC_CHANNEL_1
#define BLNK_CHANNEL  LEDC_CHANNEL_2
#define GCLK_TIMER    LEDC_TIMER_1
#define BLNK_TIMER    LEDC_TIMER_2
#define GCLK_RES      LEDC_TIMER_10_BIT
#define BLNK_RES      LEDC_TIMER_10_BIT

#define MAX_VALUE 511

void dimmerInit(void);
void setDimmer(int gs);
void setGrayscale(void);
void dimmerShift(uint16_t value);
void init_pwm_channels(void);
