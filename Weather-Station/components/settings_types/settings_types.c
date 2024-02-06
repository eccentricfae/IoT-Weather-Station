#include "settings_types.h"

#define TAG "SETTINGS_TYPES_COMPONENT"

void free_settings(basic_settings_t * settings)
{
    (void) xSemaphoreTake(settings->mutex, portMAX_DELAY);

    if (settings->wifi_name != NULL) {
        free(settings->wifi_name);
        settings->wifi_name = NULL;
    }

    if (settings->wifi_password != NULL) {
        free(settings->wifi_password);
        settings->wifi_password = NULL;
    }

    if (settings->mdns_hostname != NULL) {
        free(settings->mdns_hostname);
        settings->mdns_hostname = NULL;
    }

    if (settings->tz_code != NULL) {
        free(settings->tz_code);
        settings->tz_code = NULL;
    }
    (void) xSemaphoreGive(settings->mutex);
}

esp_err_t init_settings_struct(basic_settings_t * settings)
{
    if (settings == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    settings->mutex = xSemaphoreCreateMutex();
    if (settings->mutex == NULL) {
        ESP_LOGE(TAG, "Failed to create basic_settings_t's mutex");
        return ESP_FAIL;
    }

    return ESP_OK;
}
