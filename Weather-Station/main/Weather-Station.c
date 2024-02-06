#include <stdio.h>

#include <esp_event.h>
#include <nvs_flash.h>
#include <mdns.h>
#include <esp_timer.h>
#include <esp_log.h>

#include "wifi.h"
#include "server.h"
#include "helpers.h"
#include "sd_card.h"
#include "i2c_devices.h"

#define TAG "MAIN"
#define MAIN_LOOP_INTERVAL_MS 500

/**
 * @brief Helper function for setting the mdns hostname
 * @param hostname Hostname to be set
 * @return esp_err_t ESP_OK if succesfull or error code (check logs)
 */
esp_err_t set_mdns_hostname(char const * hostname);

void wait_for_user_reset();


void app_main(void)
{
    esp_err_t result = ESP_OK;

    // Init the Non Volatile Storage
    result = nvs_flash_init();
    if (result != ESP_OK) {
        nvs_flash_erase();
        result = nvs_flash_init();
    }
    if (result != ESP_OK) {
        wait_for_user_reset();
    }

    // Initialize the TCP/IP stack
    result = esp_netif_init();
    if (result != ESP_OK) {
        wait_for_user_reset();
    }

    // Create a default event loop which enables the system events to be sent to the event task
    result = esp_event_loop_create_default();
    if (result != ESP_OK) {
        wait_for_user_reset();
    }

    result = i2c_devices_init();
    if (result != ESP_OK) {
        wait_for_user_reset();
    }

    result = LCD_init(1000/*Refresh time in ms*/, 3/*Task priority*/);
    if (result != ESP_OK) {
        wait_for_user_reset();
    }

    basic_settings_t user_settings = {0};
    result = init_settings_struct(&user_settings);
    if (result != ESP_OK) {
        char error_msg[3][20] = {
            "Code Software 1",
            "Try resetting the",
            "device"
        };
        (void) LCD_switch_screen_error(error_msg);
        wait_for_user_reset();
    }

    uptime_datetime_t uptime = {0};
    uptime.mutex = xSemaphoreCreateMutex();
    if (uptime.mutex == NULL) {
        char error_msg[3][20] = {
            "Code Software 2",
            "Try resetting the",
            "device"
        };
        (void) LCD_switch_screen_error(error_msg);
        wait_for_user_reset();
    }

    spi_bus_config_t sd_card_spi_config = {
        .mosi_io_num   = 19,
        .miso_io_num   = 4,
        .sclk_io_num   = 18,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    gpio_num_t cs_pin = 21;

    result = sd_card_init(&sd_card_spi_config, cs_pin);
    if (result != ESP_OK) {
        char error_msg[3][20] = {
            "Code Hardware X",
            "SD Card -> out then",
            "in and reset device"
        };
        error_msg[0][14] = result == ESP_FAIL ? '1' : '2';
        (void) LCD_switch_screen_error(error_msg);
        wait_for_user_reset();
    }

    result = sd_card_read_settings(&user_settings);
    if (result != ESP_OK) {
        char error_msg[3][20] = {
            "Code Software 3",
            "Check settings file",
            "on the SD Card"
        };
        (void) LCD_switch_screen_error(error_msg);
        wait_for_user_reset();
    }

    esp_log_set_level_master(user_settings.serial_logging_verbosity);

    i2c_devices_daq_task_config_t i2c_daq_config = {
        .daq_interval_ms    = user_settings.i2c_devices_daq_interval_ms,
        .temp_sensor_option = user_settings.i2c_devices_daq_temp_sensor_select,
        .task_priority      = 3,
    };
    i2c_devices_data_t i2c_daq_data = {0};

    result = i2c_devices_lcd_spawn_daq_task(&i2c_daq_config, &i2c_daq_data);
    if (result != ESP_OK) {
        char error_msg[3][20] = {
            "Code Software 4",
            "Try resetting the",
            "device"
        };
        (void) LCD_switch_screen_error(error_msg);
        wait_for_user_reset();
    }

    (void) xSemaphoreTake(user_settings.mutex, portMAX_DELAY);
    result = set_mdns_hostname(user_settings.mdns_hostname);
    (void) xSemaphoreGive(user_settings.mutex);
    if (result != ESP_OK) {
        char error_msg[3][20] = {
            "Code Software 5",
            "Try resetting the",
            "device"
        };
        (void) LCD_switch_screen_error(error_msg);
        wait_for_user_reset();
    }

    wifi_config_t wifi_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        }
    };
    (void) xSemaphoreTake(user_settings.mutex, portMAX_DELAY);
    (void) strncpy((char *)wifi_config.sta.ssid, user_settings.wifi_name, 31);
    (void) strncpy((char *)wifi_config.sta.password, user_settings.wifi_password, 63);
    (void) xSemaphoreGive(user_settings.mutex);

    wifi_state_t wifi_state = {0};

    wifi_state.mutex = xSemaphoreCreateMutex();
    if (wifi_state.mutex == NULL) {
        ESP_LOGE(TAG, "Failed to create wifi_state's mutex");
        char error_msg[3][20] = {
            "Code Software 6",
            "Try resetting th",
            "device"
        };
        (void) LCD_switch_screen_error(error_msg);
        wait_for_user_reset();
    }

    (void) xSemaphoreTake(user_settings.mutex, portMAX_DELAY);
    result = wifi_init(
        &wifi_config, 
        &user_settings.wifi_rssi_test_period_ms,
        2, /* RSSI Testing Task Priority */
        &wifi_state
    );
    (void) xSemaphoreGive(user_settings.mutex);
    if (result != ESP_OK) {
        char error_msg[3][20] = {
            "Code Software 7",
            "Try resetting the",
            "device"
        };
        (void) LCD_switch_screen_error(error_msg);
        wait_for_user_reset();
    }

    (void) xSemaphoreTake(user_settings.mutex, portMAX_DELAY);
    result = helpers_sync_datetime(user_settings.tz_code);
    (void) xSemaphoreGive(user_settings.mutex);
    if (result != ESP_OK) {
        char error_msg[3][20] = {
            "Code Software 8",
            "Try resetting the",
            "device"
        };
        (void) LCD_switch_screen_error(error_msg);
        wait_for_user_reset();
    }

    data_aggregate_t data_for_server = {
        .wifi_state = &wifi_state,
        .user_settings = &user_settings,
        .uptime = &uptime,
        .i2c_daq_data = &i2c_daq_data,
    };

    result = server_init(&data_for_server);
    if (result != ESP_OK) {
        char error_msg[3][20] = {
            "Code Software 9",
            "Try resetting the",
            "device"
        };
        (void) LCD_switch_screen_error(error_msg);
        wait_for_user_reset();
    }

    (void) LCD_switch_screen_main(&uptime);

    char filename[13]         = {0};
    char data_write_buff[128] = {0};
    bool wrote_csv_header     = false;
    int64_t last_write_uptime = 0;

    while (true) {
        // Update the uptime and datetime
        int64_t tmp_time_us = esp_timer_get_time();
        tmp_time_us = tmp_time_us / (1000000); // Parse it from us to s
        struct tm tmp_datetime = helpers_get_datetime();
        (void) xSemaphoreTake(user_settings.mutex, portMAX_DELAY);
        uptime.uptime_s = tmp_time_us;
        uptime.date_time = tmp_datetime;
        (void) xSemaphoreGive(user_settings.mutex);

        if (sd_card_check() != ESP_OK) {
            static bool displayed_info = false;
            if (displayed_info == false) {
                ESP_LOGE(TAG, "SD card not detected. Probably taken out by the user!");
                displayed_info = true;
            }

            char error_msg[3][20] = {
                "Code Hardware 3",
                "Put SD Card in",
                "and reset device"
            };
            (void) LCD_switch_screen_error(error_msg);
            wait_for_user_reset();
        }

        bool should_write_data_to_file = ((uint32_t)(tmp_time_us - last_write_uptime) >= (uint32_t)(user_settings.data_write_to_sdcard_interval))
                                         && (tmp_datetime.tm_year != 70 && tmp_datetime.tm_year != 69)
                                         && sd_card_check() == ESP_OK;
        if (should_write_data_to_file) {
            memset(data_write_buff, 0, sizeof(data_write_buff));

            strftime(filename, 13, "%d%m%Y.txt", &tmp_datetime);

            if (sd_card_file_exists(filename) == ESP_OK) {
                wrote_csv_header = true;
            }

            if (wrote_csv_header == false) {
                sprintf(data_write_buff, "Timepoint, Temperature [C], Relative Humidity [%%], Pressure [Pa]\n");
                result = sd_card_write(filename, data_write_buff, APPEND);
                if (result != ESP_OK) {
                    ESP_LOGE(TAG, "Failed to write the CSV header to the %s file", filename);
                    goto next_iter;
                }
                wrote_csv_header = true;
            }

            strftime(data_write_buff, 13, "%T", &tmp_datetime);
            (void) xSemaphoreTake(i2c_daq_data.mutex, portMAX_DELAY);
            sprintf(data_write_buff + 8, ",%.1f,%.1f,%.1f\n", i2c_daq_data.temperature_C, i2c_daq_data.humidity_P, i2c_daq_data.pressure_Pa);
            (void) xSemaphoreGive(i2c_daq_data.mutex);

            result = sd_card_write(filename, data_write_buff, APPEND);
            if (result != ESP_OK) {
                ESP_LOGE(TAG, "Failed to write data to the %s file", filename);
                goto next_iter;
            }
            last_write_uptime = tmp_time_us;
        }

        next_iter:
        vTaskDelay(pdMS_TO_TICKS(MAIN_LOOP_INTERVAL_MS));
    }
}


esp_err_t set_mdns_hostname(char const * hostname)
{
    // Had to be put into "main" cuz I couldnt get it to work w/ the my_mdns component
    // Like I could add it as a dependency for that component

    esp_err_t result = ESP_OK;

    result = mdns_init();
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initiate mDNS module, becuase: ");
        if (result == ESP_ERR_INVALID_STATE) {
            ESP_LOGE(TAG, "Failed to register event handler");
        } else if (result == ESP_ERR_NO_MEM) {
            ESP_LOGE(TAG, "some kind of memory error");
        } else {
            ESP_LOGE(TAG, "failed to start the mDNS task");
        }
        return result;
    }

    result = mdns_hostname_set(hostname);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set the mDNS hostname");
        if (result == ESP_ERR_INVALID_ARG) {
            ESP_LOGW(TAG, "Trying to set the hostname to \"esp\"");
            result = mdns_hostname_set("esp");
        }
        return result;
    }

    return ESP_OK;
}

void wait_for_user_reset()
{
    while (true){
        vTaskDelay(-1);
    }
}
