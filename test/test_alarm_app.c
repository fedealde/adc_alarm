/* === Inclusiones de cabeceras
 * ======================================================================== */
#include "alarm_app.h"
#include "confgs.h"
#include "esp_err.h"
#include "mock_adc_port.h"
#include "stdbool.h"
#include "unity.h"

/* === Definicion y Macros================================================= */
#define cantidad(variable, tipo) (sizeof(variable) / sizeof(tipo))

/* === Declaraciones de tipos de datos internos =========================== */

/**
 * @brief Estructura de datos para prueba de nivel de alarma
 *
 */
typedef struct {
  int current_voltage_value;
  alarm_type alarm_state;
} test_bench_t;

/* === Definiciones de variables internas
 * ======================================================================== */

int test_percentage_values[] = {50, 25, 5};

test_bench_t test_data[] = {
    {.current_voltage_value = 2500, .alarm_state = NO_ALARM},
    {.current_voltage_value = 1400, .alarm_state = ALARM_LOW_LEVEL},
    {.current_voltage_value = 700, .alarm_state = ALARM_MIDDLE_LEVEL},
    {.current_voltage_value = 100, .alarm_state = ALARM_HIGH_LEVEL}};

/* === Definiciones de funciones externas ================================= */

void setUp(void) { alarm_app_set_values(test_percentage_values); }

void tearDown(void) {}

/**
 * @brief Realiza el test de inicializacion y lo realiza exitosamente
 *
 */
void test_alarm_app_init_pass(void) {

  adc_init_ExpectAndReturn(ESP_OK);

  TEST_ASSERT_EQUAL(ESP_OK, alarm_app_init());
}

/**
 * @brief Realiza el test de inicializacion y lo falla
 *
 */
void test_alarm_app_init_fail(void) {

  adc_init_ExpectAndReturn(ESP_FAIL);

  TEST_ASSERT_EQUAL(ESP_FAIL, alarm_app_init());
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

  int current_percentage_value_local = -1;
  int current_voltage_value_local = 1540;
  int current_percentage_value_expected = 50;
  int adc_voltage_local = 0;

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

  int current_percentage_value_local = -1;
  int adc_voltage_local = 0;

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

  int current_percentage_value_local = -1;
  int current_voltage_value_local = -100;
  int adc_voltage_local = 0;

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

  TEST_ASSERT_EQUAL(ESP_FAIL, alarm_app_get_abs(NULL));
}

/**
 * @brief Se realiza el test de los 4 niveles posibles de alarma para distintos
 * valores ejemplos. Al utilizarse adc_read dentro de alarm_app_get_abs, se debe
 * establecer los valores de retorno al ser llamada. Finalmente dan todos ESP_OK
 * y se imprime el valor final de la alarma para verificar su valor
 *
 */
void test_alarm_app_get_state_differen_alarms(void) {

  int adc_voltage_local = 0, indice;
  alarm_type alarm_state_local;
  char msg[30];

  for (int indice = 0; indice < (cantidad(test_data, test_bench_t)); indice++) {

    test_bench_t *test_data_local = &test_data[indice];

    adc_read_ExpectAndReturn(&adc_voltage_local, ESP_OK);
    adc_read_ReturnThruPtr_adc_voltage(
        (int *)&(test_data_local->current_voltage_value));
    TEST_ASSERT_EQUAL(ESP_OK, alarm_app_get_state(&alarm_state_local));
    TEST_ASSERT_EQUAL(test_data_local->alarm_state, alarm_state_local);
    sprintf(msg, "Indice %d alarm_state_local %d", indice, alarm_state_local);
    TEST_MESSAGE(msg);
  }
}

/**
 * @brief Se realiza el test de obtener el estado de alarma con puntero nulo
 *
 */
void test_alarm_app_get_state_fail_null_argument(void) {

  TEST_ASSERT_EQUAL(ESP_FAIL, alarm_app_get_state(NULL));
}

/**
 * @brief Se realiza el test de la obtención del nivel de alarma. Al utilizarse
 * adc_read dentro de alarm_app_get_abs, se debe stablecer los valores de
 * retorno al ser llamada. Finalmente este da ESP_FAIL por lo que la funcion
 * alarm_app_get_state falla retornando ESP_FAIL
 *
 */
void test_alarm_app_get_state_alarm_app_get_abs_return_fail(void) {

  int adc_voltage_local = 0;
  alarm_type alarm_state_local;

  adc_read_ExpectAndReturn(&adc_voltage_local, ESP_FAIL);
  TEST_ASSERT_EQUAL(ESP_FAIL, alarm_app_get_state(&alarm_state_local));
}
