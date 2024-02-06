#include "helpers.h"

#define TAG "HELPERS_COMPONENT"

esp_err_t helpers_sync_datetime(char const * tz_code)
{
    ESP_LOGI(TAG, "Setting the timezone code to %s", tz_code);
    (void) setenv("TZ", tz_code, 1);
    (void) tzset();

    esp_sntp_config_t ntp_config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    
    esp_err_t result = esp_netif_sntp_init(&ntp_config);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "esp_netif_sntp_init failed. Don't know why");
        return result;
    }

    return ESP_OK;
}

struct tm helpers_get_datetime()
{
    time_t now = {0};
    struct tm timeinfo = {0};

    (void) time(&now);
    (void) localtime_r(&now, &timeinfo);

    return timeinfo;
}
