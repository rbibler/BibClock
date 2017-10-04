#include "nettime.h"

bool alarmSet;
bool alarmOn;
int alarmHour;
int alarmMinutes;
int alarmOffTime;
int alarmLengthInMins;

#define ALARM_OFF           0x00
#define ALARM_SET           0x01
#define ALARM_ON            0x02
#define ALARM_PERIOD_ENDED  0x03

time_t now;
struct tm timeinfo;
int alarmState;

void initTimeManager(void);
void setAlarm(int hour, int minutes);
void setAlarmOffTime(int timeInMinutes);
bool checkAlarmOn(void);
void calculateAlarmOffTime(void);
int getTimeInMinutes(void);
