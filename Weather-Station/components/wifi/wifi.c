#include "wifi.h"

#define TAG "WIFI_COMPONENT"

void wifi_event_handler(void *  arg, 
                        esp_event_base_t event_base,
                        int32_t event_id, 
                        void *  event_data)
{
    if (event_base != WIFI_EVENT) {
        return;
    }

    wifi_state_t * wifi_state = (wifi_state_t *)arg;

    if (event_id == WIFI_EVENT_STA_START) {
        // uC as wifi station started
        // Try to connect to the wifi
        esp_wifi_connect();
    } else if (event_id == WIFI_EVENT_STA_CONNECTED) {
        // Station connected to the AP
        ESP_LOGI(TAG, "Device connected to a WiFi network");
        (void) xSemaphoreTake(wifi_state->mutex, portMAX_DELAY);
        wifi_state->is_connected = true;
        (void) xSemaphoreGive(wifi_state->mutex);
    } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
        // Station disconnected from the AP
        ESP_LOGW(TAG, "Device disconnected from WiFi network");
        (void) xSemaphoreTake(wifi_state->mutex, portMAX_DELAY);
        wifi_state->is_connected = false;
        (void) xSemaphoreGive(wifi_state->mutex);

        // Try to connect again
        esp_wifi_connect();
    }
}

void ip_event_handler(void *  arg, 
                      esp_event_base_t event_base,
                      int32_t event_id, 
                      void *  event_data)
{
    if (event_base != IP_EVENT) {
        return;
    }

    wifi_state_t * wifi_state = (wifi_state_t *)arg;

    if (event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "Device got IP address");
        (void) xSemaphoreTake(wifi_state->mutex, portMAX_DELAY);
        wifi_state->got_ip = true;
        ip_event_got_ip_t * event = (ip_event_got_ip_t *)event_data;
        sprintf(wifi_state->ip, IPSTR, IP2STR(&event->ip_info.ip));
        (void) xSemaphoreGive(wifi_state->mutex);
    } else if (event_id == IP_EVENT_STA_LOST_IP) {
        ESP_LOGW(TAG, "Device lost IP address");
        (void) xSemaphoreTake(wifi_state->mutex, portMAX_DELAY);
        wifi_state->got_ip = false;
        (void) xSemaphoreGive(wifi_state->mutex);
    }
}

typedef struct {
    uint32_t test_period_ms;
    wifi_state_t * out_state;
} rssi_test_task_params_t;

void rssi_test_task_entry(void * args)
{
    rssi_test_task_params_t * params = (rssi_test_task_params_t *)args;
    uint32_t delay_time_ms = params->test_period_ms;
    esp_err_t result = ESP_OK;
    int rssi_tmp = 0;

    while (true) {
        (void) xSemaphoreTake(params->out_state->mutex, portMAX_DELAY);
        
        if (params->out_state->is_connected) {
            result = esp_wifi_sta_get_rssi(&rssi_tmp);
            ESP_LOGV(TAG, "RSSI: %d", rssi_tmp);

            if (result == ESP_OK) {
                params->out_state->wifi_rssi = rssi_tmp;
            }
        }

        (void) xSemaphoreGive(params->out_state->mutex);
        
        vTaskDelay(delay_time_ms / portTICK_PERIOD_MS );
    }
}


esp_err_t wifi_init(wifi_config_t const * config, 
                    uint32_t const * rssi_test_period_ms, 
                    int32_t rssi_test_task_priority, 
                    wifi_state_t * wifi_state) 
{
    esp_err_t result = ESP_OK;

    if (config == NULL || wifi_state == NULL) {
        ESP_LOGE(TAG, "Passed invalid (NULL pointers) arguments to the wifi_init function");
        return ESP_ERR_INVALID_ARG;
    }

    if (wifi_state->mutex == NULL) {
        ESP_LOGE(TAG, "wifi_state's mutex is NULL");
        return ESP_ERR_INVALID_ARG;
    }
    
    // Creates default WIFI STA. In case of any init error this API aborts. 
    (void) esp_netif_create_default_wifi_sta();
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    
    // Initialize WiFi Allocate resource for WiFi driver, such as WiFi control structure, RX/TX buffer, WiFi NVS structure etc. This WiFi also starts WiFi task.
    result = esp_wifi_init(&cfg);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Could not initialize esp's wifi module becuase: ");
        if (result == ESP_ERR_NO_MEM) {
            ESP_LOGE(TAG, "out of memory");
        } else {
            ESP_LOGE(TAG, "unknown?");
        }
        return result;
    }

    // Set event handlers for WIFI events
    result = esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &wifi_event_handler,
        wifi_state,
        NULL
    );

    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Could not register wifi event handler, becuase: ");
        if (result == ESP_ERR_NO_MEM) {
            ESP_LOGE(TAG, "cannot allocate memory for the handler");
        } else if (result == ESP_ERR_INVALID_ARG) {
            ESP_LOGE(TAG, "invalid combination of event base and event ID or instance is NULL");
        } else {
            ESP_LOGE(TAG, "unknown error");
        }

        return result;
    }

    // Set event handlers for IP events
    result = esp_event_handler_instance_register(
        IP_EVENT,
        ESP_EVENT_ANY_ID,
        &ip_event_handler,
        wifi_state,
        NULL
    );

    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Could not register wifi event handler, becuase: ");
        if (result == ESP_ERR_NO_MEM) {
            ESP_LOGE(TAG, "cannot allocate memory for the handler");
        } else if (result == ESP_ERR_INVALID_ARG) {
            ESP_LOGE(TAG, "invalid combination of event base and event ID or instance is NULL");
        } else {
            ESP_LOGE(TAG, "unknown error");
        }

        return result;
    }

    result = esp_wifi_set_mode(WIFI_MODE_STA);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Could not set wifi to station mode, becuase: ");
        if (result == ESP_ERR_WIFI_NOT_INIT) {
            ESP_LOGE(TAG, "wifi is not initialized by the esp_wifi_init");
        } else if (result == ESP_ERR_INVALID_ARG) {
            ESP_LOGE(TAG, "invalid argument");
        } else {
            ESP_LOGE(TAG, "unknown error");
        }

        return result;
    }

    result = esp_wifi_set_config(WIFI_IF_STA, config);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Could not set wifi to station mode, becuase: ");
        if (result == ESP_ERR_WIFI_NOT_INIT) {
            ESP_LOGE(TAG, "wifi is not initialized by the esp_wifi_init");
        } else if (result == ESP_ERR_INVALID_ARG) {
            ESP_LOGE(TAG, "invalid argument");
        } else if (result == ESP_ERR_WIFI_IF) {
            ESP_LOGE(TAG, "invalid interface");
        } else if (result == ESP_ERR_WIFI_MODE) {
            ESP_LOGE(TAG, "invalid mode");
        } else if (result == ESP_ERR_WIFI_PASSWORD) {
            ESP_LOGE(TAG, "invalid password");
        } else if (result == ESP_ERR_WIFI_NVS) {
            ESP_LOGE(TAG, "wifi internal NVS error");
        } else {
            ESP_LOGE(TAG, "unknown error");
        } 

        return result;
    }

    result = esp_wifi_start();
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start the wifi module with the set configuration");
        if (result == ESP_ERR_WIFI_NOT_INIT) {
            ESP_LOGE(TAG, "wifi is not initialized by the esp_wifi_init");
        } else if (result == ESP_ERR_INVALID_ARG) {
            ESP_LOGE(TAG, "invalid argument");
        } else if (result == ESP_ERR_NO_MEM) {
            ESP_LOGE(TAG, "out of memory");
        } else if (result == ESP_ERR_WIFI_CONN) {
            ESP_LOGE(TAG, "wifi internal error, station or soft-AP control block wrong");
        } else {
            ESP_LOGE(TAG, "unknown error");
        }
        
        return result;
    }

    static rssi_test_task_params_t rssi_test_task_params = {0};

    rssi_test_task_params.test_period_ms = rssi_test_period_ms != NULL ? *rssi_test_period_ms : RSSI_TEST_PERIOD_MS_DEFAULT;
    rssi_test_task_params.out_state      = wifi_state;

    BaseType_t task_result = xTaskCreate(
        rssi_test_task_entry,
        "WIFI RSSI TEST THREAD",
        1024,
        &rssi_test_task_params,
        rssi_test_task_priority,
        NULL // Pointer - where the task handle should be stored
    );

    if (task_result != pdPASS) {
        ESP_LOGE(TAG, "Failed to initiate the WIFI RSSI TEST task");
        return ESP_FAIL;
    }

    return ESP_OK;
}
