#ifndef HELPERS_H
#define HELPERS_H

#include <esp_log.h>
#include <esp_sntp.h>
#include <esp_netif_sntp.h>


typedef struct {
    int64_t uptime_s;
    struct tm date_time;
    SemaphoreHandle_t mutex;
} uptime_datetime_t;

/**
 * @brief Synchronizes the device's local clock using the SNTP protocole.
 * Needs an active internet connection to work!
 * @param tz_code Timezone code. The format of the time string is the same as described in the GNU libc documentation 
 * @return esp_err_t ESP_OK if succesfull or an error code (check logs)
 */
esp_err_t helpers_sync_datetime(char const * tz_code);

/**
 * @brief Returns the device's current datetime
 * @return struct tm Current datetime
 */
struct tm helpers_get_datetime();


#endif