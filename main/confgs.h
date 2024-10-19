#ifndef CONFGS_H
#define CONFGS_H

//#include "confgs.h"

////// BEGIN - Alarm configurations
// Alarm quantity levels
#define ALARM_APP_QTY_LEVELS 3

// Alarm levels detection percentage
#define ALARM_APP_AL0 50
#define ALARM_APP_AL1 25
#define ALARM_APP_AL2 5

// Max and min raw map adc parameters
#define ALARM_APP_MAX_RAW_MAP 3080
#define ALARM_APP_MIN_RAW_MAP 0

// Max and min percentage map adc parameters
#define ALARM_APP_MAX_PERC_MAP 100
#define ALARM_APP_MIN_PERC_MAP 0
////// END -Alarm configurations

////// BEGIN - ADC configurations
//ADC port for alarm configuration
#define ADC_UNIT ADC_UNIT_1
#define ADC_CH ADC_CHANNEL_3
////// END -ADC configurations

#ifdef __cplusplus
extern "C" {
#endif




#ifdef __cplusplus
}
#endif

#endif //CONFGS_H