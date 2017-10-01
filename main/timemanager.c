#include "timemanager.h"

void initTimeManager(void) {
  time(&now);
  localtime_r(&now, &timeinfo);
  if(timeinfo.tm_year < (2016 - 1900)) {
    ESP_LOGI("example", "Time is not set yet. Connection to WiFi and getting time over NTP.");
    obtain_time(now, timeinfo);
    time(&now);
  }
  char strftime_buf[64];
  setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
  tzset();
  localtime_r(&now, &timeinfo);
  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
  ESP_LOGI("example", "The current date/time in New York is: %s", strftime_buf);
}

void setAlarm(int hour, int minutes) {
  alarmHour = hour;
  alarmMinutes = minutes;
  alarmSet = true;
  calculateAlarmOffTime();
}

void setAlarmOffTime(int timeInMinutes) {
  alarmLengthInMins = timeInMinutes;
  calculateAlarmOffTime();
}

void calculateAlarmOffTime(void) {
  alarmOffTime = (alarmHour * 60) + alarmMinutes + alarmLengthInMins;
}

bool checkAlarmOn(void) {
  if(!alarmSet) {
    alarmOn = false;
  } else if(alarmOn) {
    if(getTimeInMinutes() >= alarmOffTime) {
      alarmOn = false;
    }
  } else {
    alarmOn = (timeinfo.tm_hour == alarmHour) && (timeinfo.tm_min == alarmMinutes);
  }
  return alarmOn;
}

int getTimeInMinutes() {
  return (timeinfo.tm_hour * 60) + timeinfo.tm_min;
}
