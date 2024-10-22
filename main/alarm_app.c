#include "alarm_app.h"
#include "adc_port.h"
#include "confgs.h"
#include <stdbool.h>

//#define DEBUG

#ifdef DEBUG

const static char *TAG = "alarm_app.c";

#define ASSERT_NULL(arg)                                                       \
  if (arg) {                                                                   \
    printf("%s: error \"NULL POINTER\", \nfile: \"%s\"\nline: "                \
           "%d\nfunc:\"%s\"\n",                                                \
           TAG, __FILE__, __LINE__, __ASSERT_FUNC);                            \
    return ESP_FAIL;                                                           \
  }

#else
#define ASSERT_NULL(arg)                                                       \
  if (arg) {                                                                   \
    return ESP_FAIL;                                                           \
  }
#endif

static int alarm_app_map(int *voltage_value_to_map);
static bool alarm_app_compare_limits(int alarm_state);

static int alarm_threshold_vector[ALARM_APP_QTY_LEVELS] = {0};

esp_err_t alarm_app_init(void) { return adc_init(); }

esp_err_t alarm_app_set_values(int *percentage_values) {

  for (int cnt_aux = 0; cnt_aux < ALARM_APP_QTY_LEVELS; cnt_aux++) {

    if (alarm_app_compare_limits(percentage_values[cnt_aux])) {
      alarm_threshold_vector[cnt_aux] = percentage_values[cnt_aux];
    } else {
      return ESP_FAIL;
    }
  }
  return ESP_OK;
}

esp_err_t alarm_app_get_abs(int *current_percentage_value) {

  int current_voltage_value = 0;

  ASSERT_NULL(NULL == current_percentage_value);

  if (ESP_OK != adc_read(&current_voltage_value)) {
    return ESP_FAIL;
  }

  if (ALARM_APP_MAX_RAW_MAP <= current_voltage_value) {
    current_voltage_value = ALARM_APP_MAX_RAW_MAP;
  }

  *current_percentage_value = alarm_app_map(&current_voltage_value);

  if (alarm_app_compare_limits(*current_percentage_value)) {
    return ESP_OK;
  } else {
    return ESP_FAIL;
  }
}

esp_err_t alarm_app_get_state(alarm_type *alarm_state) {

  int current_percentage_value = 0;

  ASSERT_NULL(NULL == alarm_state);

  if (ESP_OK == alarm_app_get_abs(&current_percentage_value)) {

    if (alarm_threshold_vector[0] >= current_percentage_value &&
        alarm_threshold_vector[1] < current_percentage_value) {
      *alarm_state = ALARM_LOW_LEVEL;
    } else if (alarm_threshold_vector[1] >= current_percentage_value &&
               alarm_threshold_vector[2] < current_percentage_value) {
      *alarm_state = ALARM_MIDDLE_LEVEL;
    } else if (alarm_threshold_vector[2] >= current_percentage_value &&
               ALARM_APP_MIN_PERC_MAP <= current_percentage_value) {
      *alarm_state = ALARM_HIGH_LEVEL;
    } else {
      *alarm_state = NO_ALARM;
    }
    return ESP_OK;

  } else {
    return ESP_FAIL;
  }
}

static int alarm_app_map(int *voltage_value_to_map) {

  ASSERT_NULL(NULL == voltage_value_to_map);

  return ((int)((*voltage_value_to_map - ALARM_APP_MIN_RAW_MAP) *
                (ALARM_APP_MAX_PERC_MAP - ALARM_APP_MIN_PERC_MAP) /
                (ALARM_APP_MAX_RAW_MAP - ALARM_APP_MIN_RAW_MAP)) +
          ALARM_APP_MIN_RAW_MAP);
}

static bool alarm_app_compare_limits(int alarm_state) {

  if (ALARM_APP_MAX_PERC_MAP >= alarm_state &&
      ALARM_APP_MIN_PERC_MAP <= alarm_state) {
    return true;
  }

  return false;
}