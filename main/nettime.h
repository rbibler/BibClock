#include "lwip/err.h"
#include "apps/sntp/sntp.h"
#include "wifimanager.h"

void netTimeUpdate(void);
void obtain_time(time_t now, struct tm timeinfo);
void initialize_sntp(void);
