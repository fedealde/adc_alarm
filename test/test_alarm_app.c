#include "alarm_app.h"
#include "confgs.h"
#include "esp_err.h"
#include "mock_adc_port.h"
#include "stdbool.h"
#include "unity.h"

void setUp(void) {}

void tearDown(void) {}

/**
 * @brief Realiza el test de inicializacion y lo realiza exitosamente
 *
 */
void test_alarm_app_init_fail(void) {

  adc_init_ExpectAndReturn(ESP_OK);

  TEST_ASSERT_EQUAL(ESP_OK, alarm_app_init());
}

/**
 * @brief Realiza el test de la asignacion de valores porcentuales dentro
 * del vector de valores limites de alarma correspondiente a los niveles
 * designados dentro del modulo (nivel 0, 1, 2 y 3) respectivamente. Pasa el
 * test ya que los valores van de 0 a 100
 *
 */
void test_alarm_app_set_values_pass(void) {

  int test_percentage_values[] = {50, 25, 5};

  TEST_ASSERT_EQUAL(ESP_OK, alarm_app_set_values(test_percentage_values));
}

/**
 * @brief Realiza el test de la asignacion de valores porcentuales dentro
 * del vector de valores limites de alarma correspondiente a los niveles
 * designados dentro del modulo (nivel 0, 1, 2 y 3) respectivamente. Falla el
 * test ya que el segundo valor es 101 y es mayor a 100
 *
 */
void test_alarm_app_set_values_pass_over_hundred_and_under_zero_and_limits(
    void) {

  int test_percentage_values_over_hundred[] = {101, 50, 1};
  int test_percentage_values_under_zero[] = {99, 50, -5};
  int test_percentage_values_limits[] = {100, 49, 0};

  TEST_ASSERT_EQUAL(ESP_FAIL,
                    alarm_app_set_values(test_percentage_values_over_hundred));
  TEST_ASSERT_EQUAL(ESP_FAIL,
                    alarm_app_set_values(test_percentage_values_under_zero));
  TEST_ASSERT_EQUAL(ESP_OK,
                    alarm_app_set_values(test_percentage_values_limits));
}