#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adc_port.h"
#include "alarm_app.h"
#include "confgs.h"

const static char *TAG = "main.c";

void app_main(void) {

  int alarm_config[ALARM_APP_QTY_LEVELS] = {ALARM_APP_AL0, ALARM_APP_AL1,
                                            ALARM_APP_AL2};
  alarm_type alarm_state = NO_ALARM;

  ESP_ERROR_CHECK(alarm_app_init());

  alarm_app_set_values(alarm_config);

  while (1) {

    ESP_ERROR_CHECK(alarm_app_get_state(&alarm_state));
    ESP_LOGI(TAG, "Alarm level %d", alarm_state);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
