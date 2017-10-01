#include "nettime.h"

bool alarmSet;
bool alarmOn;
int alarmHour;
int alarmMinutes;
int alarmOffTime;
int alarmLengthInMins;

time_t now;
struct tm timeinfo;

void initTimeManager(void);
void setAlarm(int hour, int minutes);
void setAlarmOffTime(int timeInMinutes);
bool checkAlarmOn(void);
void calculateAlarmOffTime(void);
int getTimeInMinutes(void);
