#include "nettime.h"

int alarmHour;
int alarmMinutes;
int alarmOffTime;
int alarmTimeoutTimeStart;
int alarmLengthInMins;
int alarmTimeoutTime;

#define ALARM_OFF           0x00
#define ALARM_SET           0x01
#define ALARM_ON            0x02
#define ALARM_PERIOD_ENDED  0x03

time_t now;
struct tm timeinfo;
int alarmState;

void initTimeManager(void);
void setAlarm(int hour, int minutes, int lengthInMinutes);
void setAlarmTimeout(int timeOutInMinutes);
bool checkAlarmOn(void);
void calculateAlarmOffTime(void);
void calculateAlarmTimeOutBeginTime(void);
int getTimeInMinutes(void);
