#ifndef ALARM_APP_H
#define ALARM_APP_H

//#include "alarm_app.h"

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Alarm_type type for alarm levels
 *
 */
typedef enum alarm {
  NO_ALARM = 0,
  ALARM_LOW_LEVEL,
  ALARM_MIDDLE_LEVEL,
  ALARM_HIGH_LEVEL,
} alarm_type;

/**
 * @brief Alarm app initialization
 *
 * @return esp_err_t
 */
esp_err_t alarm_app_init(void);

/**
 * @brief Set alarm threshold values
 *
 * @param percentage_values Vector pointer of percentage threshold values
 * @return esp_err_t
 */
esp_err_t alarm_app_set_values(int *percentage_values);

/**
 * @brief Get the absolute percentage level
 *
 * @param current_percentage_value Pointer to be filled with absolute percentage
 * level
 * @return esp_err_t
 */
esp_err_t alarm_app_get_abs(int *current_percentage_value);

/**
 * @brief Get the alarm state
 *
 * @param alarm_state Pointer to be filled with alarm state
 * @return esp_err_t
 */
esp_err_t alarm_app_get_state(alarm_type *alarm_state);

#ifdef __cplusplus
}
#endif

#endif // ALARM_APP_H