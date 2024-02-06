#ifndef SETTINGS_TYPES_H
#define SETTINGS_TYPES_H

#include <esp_log.h>
#include <freertos/FreeRTOS.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIFI_NAME_MAX_LEN 48
#define WIFI_PASS_MAX_LEN 48
#define MDNS_HOSTNAME_MAX_LEN 32
#define TZ_CODE_MAX_LEN 48
#define SD_CARD_LOGGING_PATH_MAX_LEN 64

typedef struct {
    bool   was_generated_by_default;
    char * wifi_name; // max len 31
    char * wifi_password; // max len 63
    char * mdns_hostname;
    char * tz_code;
    esp_log_level_t serial_logging_verbosity;
    uint32_t wifi_rssi_test_period_ms;
    uint32_t i2c_devices_daq_interval_ms;
    uint32_t i2c_devices_daq_temp_sensor_select;
    uint32_t data_write_to_sdcard_interval;
    SemaphoreHandle_t mutex;
} basic_settings_t;

/**
 * @brief Frees the basic_settings_t's field's dynamically allocated memory
 */
void free_settings(basic_settings_t * settings);

/**
 * @brief Initializes basic_settings_t's mutex
 * @return esp_err_t ESP_OK or error code
 */
esp_err_t init_settings_struct(basic_settings_t * settings);


#endif