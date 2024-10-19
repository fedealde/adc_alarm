#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adc_port.h"
#include "confgs.h"

const static char *TAG = "adc_port.c";

#define ADC_BW ADC_BITWIDTH_DEFAULT
#define ADC_ATT ADC_ATTEN_DB_12

#define ADC_ERROR_CHECK_AND_RETURN(x)                                          \
  do {                                                                         \
    esp_err_t err_rc_ = (x);                                                   \
    if (err_rc_ != ESP_OK) {                                                   \
      printf("%s: error \"%s\", esp_err_t code 0x%x, \nfile: \"%s\"\nline: "   \
             "%d\nfunc:\"%s\"\nexpression: %s\n",                              \
             TAG, esp_err_to_name(err_rc_), err_rc_, __FILE__, __LINE__,       \
             __ASSERT_FUNC, #x);                                               \
      return err_rc_;                                                          \
    }                                                                          \
  } while (0)

static bool adc_calibration_init(adc_unit_t unit, adc_channel_t channel,
                                 adc_atten_t atten,
                                 adc_cali_handle_t *out_handle);

static esp_err_t adc_calibration_deinit(adc_cali_handle_t handle);

bool calibration_flag;

adc_oneshot_unit_handle_t adc1_handle;
adc_oneshot_unit_init_cfg_t init_config1 = {
    .unit_id = ADC_UNIT,
};

adc_cali_handle_t adc1_cali_chan1_handle = NULL;

adc_oneshot_chan_cfg_t config = {
    .bitwidth = ADC_BW,
    .atten = ADC_ATT,
};

esp_err_t adc_init(void) {

  ADC_ERROR_CHECK_AND_RETURN(adc_oneshot_new_unit(&init_config1, &adc1_handle));

  ADC_ERROR_CHECK_AND_RETURN(
      adc_oneshot_config_channel(adc1_handle, ADC_CH, &config));

  calibration_flag =
      adc_calibration_init(ADC_UNIT, ADC_CH, ADC_ATT, &adc1_cali_chan1_handle);

  return ESP_OK;
}

esp_err_t adc_read(int *adc_voltage) {

  int adc_raw;

  ADC_ERROR_CHECK_AND_RETURN(adc_oneshot_read(adc1_handle, ADC_CH, &adc_raw));

  ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT + 1, ADC_CH,
           adc_raw);

  if (calibration_flag) {
    ADC_ERROR_CHECK_AND_RETURN(
        adc_cali_raw_to_voltage(adc1_cali_chan1_handle, adc_raw, adc_voltage));

    ESP_LOGI(TAG, "ADC%d Channel[%d] Cali Voltage: %d mV", ADC_UNIT + 1, ADC_CH,
             *adc_voltage);
  }

  return ESP_OK;
}

esp_err_t adc_deinit(void) {

  ADC_ERROR_CHECK_AND_RETURN(adc_oneshot_del_unit(adc1_handle));

  if (calibration_flag) {
    ADC_ERROR_CHECK_AND_RETURN(adc_calibration_deinit(adc1_cali_chan1_handle));
  }

  return ESP_OK;
}

static bool adc_calibration_init(adc_unit_t unit, adc_channel_t channel,
                                 adc_atten_t atten,
                                 adc_cali_handle_t *out_handle) {
  adc_cali_handle_t handle = NULL;
  esp_err_t ret = ESP_FAIL;
  bool calibrated = false;

  if (!calibrated) {
    ESP_LOGI(TAG, "calibration scheme version is %s", "Curve Fitting");
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = unit,
        .chan = channel,
        .atten = atten,
        .bitwidth = ADC_BW,
    };
    ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
    if (ret == ESP_OK) {
      calibrated = true;
    }
  }

  *out_handle = handle;
  if (ret == ESP_OK) {
    ESP_LOGI(TAG, "Calibration Success");
  } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
    ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
  } else {
    ESP_LOGE(TAG, "Invalid arg or no memory");
  }

  return calibrated;
}

static esp_err_t adc_calibration_deinit(adc_cali_handle_t handle) {

  ESP_LOGI(TAG, "deregister %s calibration scheme", "Curve Fitting");
  ADC_ERROR_CHECK_AND_RETURN(adc_cali_delete_scheme_curve_fitting(handle));

  return ESP_OK;
}