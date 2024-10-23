# _Trabajo Practico N° 3 - TSSE - CESE UBA_

>Autor: Federico Alderisi

Desarrollo de módulo de alarma de nivel mediante adc y su posterior testing. Este módulo se utiliza en el trabajo final del posgrado para medir el nivel de un tanque.

## Detector de nivel con alarma

-   El programa consta de un módulo adc_port que permite utilizar las funcionalidades de alto nivel del adc para poder inicializar y leer los valores del adc.
-   Se utiliza un archivo llamado configs.h para definir macros y constantes que permiten configurar el sistema.
-   La aplicación del detector de nivel se desarrolla en alarm_app. Aquí se desarrollan las funcionalidades principales como: inicializar, set del nivel absoluto porcentual de alarma, get del valor absoluto porcentual del nivel y get del estado actual de la alarma (no_alarma, bajo, medio y alto) según corresponda la configuración.
-   Finalmente se realizan los tests unitarios del módulo principal alarm_app y su reporte de nivel de covertura correspondiente.

## Tests realizados y resultados

>Los resultados del gcov **`.html`** se encuentra en la carpeta **Resultados_gcov** en la raiz del proyecto

1.    Realiza el test de inicializacion y lo realiza exitosamente
2.    Realiza el test de inicializacion y lo falla
3.    Realiza el test de la asignacion de valores porcentuales dentro del vector de valores limites de alarma correspondiente a los niveles designados dentro del modulo (nivel 0, 1, 2 y 3) respectivamente. Pasa el test ya que los valores van de 0 a 100
4.    Realiza el test de la asignacion de valores porcentuales dentro del vector de valores limites de alarma correspondiente a los niveles designados dentro del modulo (nivel 0, 1, 2 y 3) respectivamente. Falla el primer test ya que el valor es 101 y es mayor a 100. El segundo falla porque es menor que cero. El tercero pasa ya que esta en el limite.
5.    La funcion mide el nivel y se guarda el valor en porcentaje en el puntero pasado como parametro. La operacion es exitosa.
6.    La funcion mide el nivel y se guarda el valor en porcentaje en el puntero pasado como parametro. La operacion falla porque el adc no lee correctamente.
7.    La funcion mide el nivel y se guarda el valor en porcentaje en el puntero pasado como parametro. La operacion falla porque el valor de los limites del map estan por fuera de lo estimado. El valor que devuelve es negativo y no se utiliza porque devuelve ESP_FAIL
8.    La funcion mide el nivel y se guarda el valor en porcentaje en el puntero pasado como parametro. La operacion falla porque el valor de los limites del map estan por fuera de lo estimado. El valor que devuelve es negativo y no se utiliza porque devuelve ESP_FAIL
9.   Se realiza el test de los 4 niveles posibles de alarma para distintos valores ejemplos. Al utilizarse adc_read dentro de alarm_app_get_abs, se debe establecer los valores de retorno al ser llamada. Finalmente dan todos ESP_OK y se imprime el valor final de la alarma para verificar su valor
10.    Se realiza el test de obtener el estado de alarma con puntero nulo
11.    Se realiza el test de la obtención del nivel de alarma. Al utilizarse adc_read dentro de alarm_app_get_abs, se debe stablecer los valores de retorno al ser llamada. Finalmente este da ESP_FAIL por lo que la funcion alarm_app_get_state falla retornando ESP_FAIL

```bash
ceedling clobber gcov:all utils:gcov
```
```
Test 'test_alarm_app.c'
-----------------------
Generating include list for adc_port.h...
Creating mock for adc_port...
Generating runner for test_alarm_app.c...
Compiling test_alarm_app_runner.c...
Compiling test_alarm_app.c...
Compiling mock_adc_port.c...
Compiling alarm_app.c with coverage...
Compiling unity.c...
Compiling cmock.c...
Linking test_alarm_app.out...
Running test_alarm_app.out...
Creating gcov results report(s) in 'build/artifacts/gcov'... Done in 1.015 seconds.

-----------------
GCOV: TEST OUTPUT
-----------------
[test_alarm_app.c]
  - "test_alarm_app.c:189:test_alarm_app_get_state_differen_alarms:INFO: Indice 0 alarm_state_local 0"
  - "test_alarm_app.c:189:test_alarm_app_get_state_differen_alarms:INFO: Indice 1 alarm_state_local 1"
  - "test_alarm_app.c:189:test_alarm_app_get_state_differen_alarms:INFO: Indice 2 alarm_state_local 2"
  - "test_alarm_app.c:189:test_alarm_app_get_state_differen_alarms:INFO: Indice 3 alarm_state_local 3"

--------------------------
GCOV: OVERALL TEST SUMMARY
--------------------------
TESTED:  11
PASSED:  11
FAILED:   0
IGNORED:  0


---------------------------
GCOV: CODE COVERAGE SUMMARY
---------------------------
alarm_app.c Lines executed:97.62% of 42
alarm_app.c Branches executed:100.00% of 34
alarm_app.c Taken at least once:91.18% of 34
alarm_app.c Calls executed:100.00% of 6
alarm_app.c Lines executed:97.62% of 42
```

### First steps

Luego de instalar los framewors utilizando los siguientes pasos:

- Instalación de Ruby
`sudo apt-get install ruby gcovr`
- Instalación de Ceedling
`sudo gem install ceedling`
- Creación del repositorio de trabajo
`git init project_name`
- Creación del proyecto ceedling
`ceedling new project_name`

Versiones de framework:

`ruby -v`
ruby 3.0.2p107 (2021-07-07 revision 0db68f0233) [x86_64-linux-gnu]
`ceedling version`
>Welcome to Ceedling!
    Ceedling:: 0.31.1
       CMock:: 2.5.4
       Unity:: 2.5.4
  CException:: 1.3.3