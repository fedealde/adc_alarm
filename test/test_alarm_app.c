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
 * primer test ya que el valor es 101 y es mayor a 100. El segundo falla porque
 * es menor que cero. El tercero pasa ya que esta en el limite.
 *
 */
void test_alarm_app_set_values_fail_over_hundred_fail_under_zero_and_pass_limits(
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

/**
 * @brief La funcion mide el nivel y se guarda el valor en porcentaje en el
 * puntero pasado como parametro. La operacion es exitosa.
 *
 */
void test_alarm_app_get_abs_pass(void) {

  int test_percentage_values[] = {50, 25, 5};
  int current_percentage_value_local = -1;
  int current_voltage_value_local = 1540;
  int current_percentage_value_expected = 50;
  int adc_voltage_local = 0;

  alarm_app_set_values(test_percentage_values);
  adc_read_ExpectAndReturn(&adc_voltage_local, ESP_OK);
  adc_read_ReturnThruPtr_adc_voltage(&current_voltage_value_local);
  TEST_ASSERT_EQUAL(ESP_OK, alarm_app_get_abs(&current_percentage_value_local));
  TEST_ASSERT_EQUAL(current_percentage_value_expected,
                    current_percentage_value_local);
}

/**
 * @brief La funcion mide el nivel y se guarda el valor en porcentaje en el
 * puntero pasado como parametro. La operacion falla porque el adc no lee
 * correctamente.
 *
 */
void test_alarm_app_get_abs_fail_adc_read(void) {

  int test_percentage_values[] = {50, 25, 5};
  int current_percentage_value_local = -1;
  int adc_voltage_local = 0;

  alarm_app_set_values(test_percentage_values);
  adc_read_ExpectAndReturn(&adc_voltage_local, ESP_FAIL);
  TEST_ASSERT_EQUAL(ESP_FAIL,
                    alarm_app_get_abs(&current_percentage_value_local));
}

/**
 * @brief La funcion mide el nivel y se guarda el valor en porcentaje en el
 * puntero pasado como parametro. La operacion falla porque el valor de los
 * limites del map estan por fuera de lo estimado. El valor que devuelve es
 * negativo y no se utiliza porque devuelve ESP_FAIL
 *
 */
void test_alarm_app_get_abs_fail_alarm_app_map(void) {

  int test_percentage_values[] = {50, 25, 5};
  int current_percentage_value_local = -1;
  int current_voltage_value_local = -100;
  int adc_voltage_local = 0;

  alarm_app_set_values(test_percentage_values);
  adc_read_ExpectAndReturn(&adc_voltage_local, ESP_OK);
  adc_read_ReturnThruPtr_adc_voltage(&current_voltage_value_local);
  TEST_ASSERT_EQUAL(ESP_FAIL,
                    alarm_app_get_abs(&current_percentage_value_local));
}

/**
 * @brief La funcion mide el nivel y se guarda el valor en porcentaje en el
 * puntero pasado como parametro. La operacion falla porque el valor de los
 * limites del map estan por fuera de lo estimado. El valor que devuelve es
 * negativo y no se utiliza porque devuelve ESP_FAIL
 *
 */
void test_alarm_app_get_abs_fail_null_argument(void) {

  int test_percentage_values[] = {50, 25, 5};

  alarm_app_set_values(test_percentage_values);
  TEST_ASSERT_EQUAL(ESP_FAIL, alarm_app_get_abs(NULL));
}
