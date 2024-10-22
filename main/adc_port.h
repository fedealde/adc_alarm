#ifndef ADC_PORT_H
#define ADC_PORT_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t adc_init(void);
esp_err_t adc_deinit(void);
esp_err_t adc_read(int *adc_voltage);

#ifdef __cplusplus
}
#endif

#endif // ADC_PORT_H