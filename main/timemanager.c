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

void setAlarm(int hour, int minutes, int lengthInMinutes) {
  alarmHour = hour;
  alarmMinutes = minutes;
  alarmLengthInMins = lengthInMinutes;
  alarmState = ALARM_SET;
  calculateAlarmTimeOutBeginTime();
}

void setAlarmTimeout(int timeOutInMinutes) {
  alarmTimeoutTime = timeOutInMinutes;
  calculateAlarmOffTime();
}

void calculateAlarmOffTime(void) {
  alarmOffTime = alarmTimeoutTimeStart + alarmTimeoutTime;
}

void calculateAlarmTimeOutBeginTime(void) {
  alarmTimeoutTimeStart = (alarmHour * 60) + alarmMinutes + alarmLengthInMins;
}

bool checkAlarmOn(void) {
  bool retValue = false;
  switch(alarmState) {
    case ALARM_SET:
      if(timeinfo.tm_hour == alarmHour && timeinfo.tm_min == alarmMinutes) {
        alarmState = ALARM_ON;
        retValue = true;
      }
    break;
    case ALARM_ON:
      if(getTimeInMinutes() >= alarmTimeoutTimeStart) {
        alarmState = ALARM_PERIOD_ENDED;
      }
      retValue = true;
    break;
    case ALARM_PERIOD_ENDED:
      if(getTimeInMinutes() >= alarmOffTime) {
        retValue = false;
        alarmState = ALARM_OFF;
      } else {
        retValue = true;
      }
    break;
  }
  return retValue;
}

int getTimeInMinutes() {
  return (timeinfo.tm_hour * 60) + timeinfo.tm_min;
}
