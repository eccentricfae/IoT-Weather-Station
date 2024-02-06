#ifndef WIFI_H
#define WIFI_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <esp_log.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>
#include <esp_netif.h>
#include <freertos/FreeRTOS.h>


#define RSSI_TEST_PERIOD_MS_DEFAULT 60000

/**
 * @brief Struct used for communicating about the current state of the wifi 
 * connection. The mutex field NEEDS to be initialized w/ a handle to a mutex
 * created specifically for guarding the instance of this struct (the mutex' 
 * parent struct instance)
 */
typedef struct {
    bool is_connected;
    bool got_ip;
    int32_t wifi_rssi;
    char ip[16];
    SemaphoreHandle_t mutex;
} wifi_state_t;

/**
 * @brief Initializes the wifi module of the ESP board. Initializes the WiFi
 * module in STATION mode. Creates event handlers for WIFI events (wifi start, 
 * wifi connection, wifi disconnection) and IP events (got / lost ip address).
 * The handlers change the state of the wifi_state variable to indicate the 
 * current state of the wifi module. The function also creates new RTOS task
 * that periodically monitors the RSSI of the WiFi connection and also stores
 * it in the wifi_state variable
 * 
 * @param config [in] Pointer to the structure w/ the configuration as to how the wifi station should be set
 * Cannot be NULL or will return error
 * @param rssi_test_period_ms [in] Pointer to an integer w/ the period (in milliseconds) for the wifi's rssi test to be run.
 * If the pointer is NULL the functions defaults to RSSI_TEST_PERIOD_MS_DEFAULT
 * @param rssi_test_task_priority Integer w/ the priority for the rssi test task 
 * @param wifi_state [out] Pointer to the structure where the current wifi state should be "placed"
 * @return esp_err_t ESP_OK If initialized the WiFi module correctly. ESP_ERR_INVALID_ARG if passed bad arguments (null pointers or invalid mutex handle). ESP_FAIL if some part of the initialization chain of functions failed.
 * Cannot be NULL or will return error
 */
esp_err_t wifi_init(wifi_config_t const * config, 
                    uint32_t const * rssi_test_period_ms, 
                    int32_t rssi_test_task_priority, 
                    wifi_state_t * wifi_state);


#endif