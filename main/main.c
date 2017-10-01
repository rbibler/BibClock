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
#include "esp_attr.h"
#include "esp_sleep.h"

#include "lwip/err.h"
#include "apps/sntp/sntp.h"
#include "wifimanager.h"
#include "timemanager.h"

uint8_t curDigitValues[] = {0,1,2,3};
uint8_t digitDots[] = {1, 0, 0, 1};
bool digitDirections[] = {false, true, false, true};
void updateDisplay(void);
long lastTime;
static const char *TAG = "example";

void updateTimeDigits(void);

void app_main(void)
{
  initTimeManager();
  sevenDigitInit();
  setAlarm(17, 18);
  setAlarmOffTime(2);
  while (true) {
    updateDisplay();
    long time = (xTaskGetTickCount() / portTICK_PERIOD_MS);
    if(time - lastTime >= 500) {
      updateTimeDigits();
      if(checkAlarmOn()) {
        digitDots[3] = 0;
      } else {
        digitDots[3] = 1;
      }
      lastTime = time;
    }
  }
}

void updateDisplay(void) {
  for(int i = 0; i < 4; i++) {
    displayDigit(i, curDigitValues[i], digitDots[i], digitDirections[i]);
    vTaskDelay(2 / portTICK_PERIOD_MS);
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
