#ifndef SERVER_H
#define SERVER_H

#include <esp_log.h>
#include <esp_http_server.h>
#include <freertos/FreeRTOS.h>

#include "wifi.h"
#include "settings_types.h"
#include "helpers.h"
#include "i2c_devices.h"
#include "sd_card.h"
#include "cJSON.h"
#include <dirent.h>
#include <sys/types.h>

/**
 * @brief Aggregate for the server with the device's data
 */
typedef struct {
    wifi_state_t * wifi_state;
    basic_settings_t * user_settings;
    uptime_datetime_t * uptime;
    i2c_devices_data_t * i2c_daq_data;
} data_aggregate_t;


/**
 * @brief Initialize the server component
 * @param data_for_server Pointer to the data_aggregate_t struct with data for the server
 * @return esp_err_t ESP_OK or error code
 */
esp_err_t server_init(data_aggregate_t * data_for_server);


#endif