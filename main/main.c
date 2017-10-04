#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "sevendigit.h"
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include <math.h>
//#include "esp_attr.h"
#include "esp_sleep.h"

#include "lwip/err.h"
#include "apps/sntp/sntp.h"
#include "wifimanager.h"
#include "timemanager.h"
#include "dimmer.h"

uint8_t curDigitValues[] = {0,1,2,3};
uint8_t digitDots[] = {1, 0, 0, 1};
bool digitDirections[] = {false, true, false, true};
void updateDisplay(void);
long lastTime;
static const char *TAG = "example";
int curDimmer = 0;

void updateTimeDigits(void);
void alarm_check(void);

void app_main(void)
{
  initTimeManager();
  sevenDigitInit();
  dimmerInit();
  setDimmer(1);
  setAlarm(7, 44, 20);
  setAlarmTimeout(4);
  while (true) {
    updateDisplay();
    long time = (xTaskGetTickCount() / portTICK_PERIOD_MS);
    if(time - lastTime >= 500) {
      updateTimeDigits();
      alarm_check();
      lastTime = time;
    }
  }
}

// This function calculates the new dimmer grayscale value using an
// exponetial growth formula to ensure gradual dimming in the lower levels
// Otherwise the brightness increases too quickly.
// rate is calculated based on growth from 1 to 512 in 20 minutes.
// TO-DO: Calculate rate based on arbitrary time.
void exponential_led_dimmer_increase(void) {
  double rateTime = .0026 * curDimmer;
  double newDim = exp(rateTime);
  setDimmer((int) newDim);
  curDimmer++;
  if(curDimmer >= 2400) {
    curDimmer = 2400;
  }
}

void alarm_check() {
  if(checkAlarmOn()) {
    // Alarm is currently on, so check if dimmer has started
    // If not, set to 1 to get things going.
    if(curDimmer == 0) {
      curDimmer = 1;
    }
    // Increase dimmer for duration of alarm. This function handles the
    // Maximum brightness so there's no overshoot.
    exponential_led_dimmer_increase();
  } else {
    // Alarm is off, so turn off the dimmer.
    if(curDimmer > 0) {
      curDimmer = 0;
      setDimmer(0);
    }
  }
}

void updateDisplay(void) {
  for(int i = 0; i < 4; i++) {
    displayDigit(i, curDigitValues[i], digitDots[i], digitDirections[i]);
    //vTaskDelay(2 / portTICK_PERIOD_MS);
  }
}

void updateTimeDigits(void) {
  time(&now);
  localtime_r(&now, &timeinfo);
  int min = timeinfo.tm_min;
  int hour = timeinfo.tm_hour;
  if(hour > 12) {
    hour -= 12;
    digitDots[0] = 0;
  } else {
    digitDots[0] = 1;
  }
  curDigitValues[0] = min % 10;
  curDigitValues[1] = min / 10;
  curDigitValues[2] = hour % 12;
  curDigitValues[3] = hour / 12;
  if(curDigitValues[3] == 0) {
    curDigitValues[3] = 10;
  }
}
