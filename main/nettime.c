#include "nettime.h"

void obtain_time(time_t now, struct tm timeinfo) {
  ESP_ERROR_CHECK( nvs_flash_init() );
  initialise_wifi();
  xEventGroupWaitBits(wifi_event_group, BIT0, false, true, portMAX_DELAY);
  initialize_sntp();

  int retry = 0;
  const int retry_count = 10;
  while(timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
    ESP_LOGI("example", "Waiting for system time to be set... (%d/%d)", retry, retry_count);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    time(&now);
    localtime_r(&now, &timeinfo);
  }

  ESP_ERROR_CHECK( esp_wifi_stop() );
}

void initialize_sntp(void) {
  ESP_LOGI("example", "Initializing SNTP");
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "pool.ntp.org");
  sntp_init();
}
