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
uint8_t digitDots[] = {0, 1, 1, 0};

bool digitDirections[] = {false, true, false, true};

long lastTime;

static const char *TAG = "example";

int curDimmer = 0;

void updateTimeDigits(void);
void alarm_check(void);

void app_main(void)
{
  initTimeManager();
  sevenDigitInit();
  displayDigit(0, 3, 1, false);
  dimmerInit();
  setDimmer(0);
  setAlarm(12, 03, 20);
  setAlarmTimeout(4);
  while (true) {

    long time = (xTaskGetTickCount() / portTICK_PERIOD_MS);
    if(time - lastTime >= 1000) {
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

void updateTimeDigits(void) {
  time(&now);
  localtime_r(&now, &timeinfo);
  int min = timeinfo.tm_min;
  int hour = timeinfo.tm_hour;
  if(hour > 12) {
    hour -= 12;
    digitDots[0] = 1;
  } else {
    digitDots[0] = 0;
  }
  int newMinOnes = min % 10;
  int newMinTens = min / 10;
  int newHrOnes = hour % 10;
  int newHrTens = hour / 10;

  if(newMinOnes != curDigitValues[0]) {
    displayDigit(0, newMinOnes, digitDots[0], digitDirections[0]);
  }
  if(newMinTens != curDigitValues[1]) {
    displayDigit(1, newMinTens, digitDots[1], digitDirections[1]);
  }
  if(newHrOnes != curDigitValues[2]) {
    displayDigit(2, newHrOnes, digitDots[2], digitDirections[2]);
  }
  if(newHrTens != curDigitValues[3]) {
    displayDigit(3, newHrTens == 0 ? SEVEN_DIGIT_BLANK : newHrTens, digitDots[3], digitDirections[3]);
  }


  curDigitValues[0] = newMinOnes;
  curDigitValues[1] = newMinTens;
  curDigitValues[2] = newHrOnes;
  curDigitValues[3] = newHrTens;

}
