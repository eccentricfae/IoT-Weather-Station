#ifndef I2C_DEVICES_H
#define I2C_DEVICES_H

#include <esp_log.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <rom/ets_sys.h>
#include <am2320.h>
#include <bmp280.h>
#include <hd44780.h>
#include <pcf8574.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "helpers.h"

#define I2C_SDA_PIN 22
#define I2C_SCL_PIN 23

typedef enum {
    TEMP_SENSOR_AM2320,
    TEMP_SENSOR_BMP280,
    TEMP_SENSOR_MEAN,
} TEMP_SENSOR_SELECT;

typedef struct {
    float temperature_C; // temperature [Celsius]
    float humidity_P;    // relative humidity [percentage]
    float pressure_Pa;   // pressure [Pascal]
    SemaphoreHandle_t mutex;
} i2c_devices_data_t;

typedef struct {
    uint32_t daq_interval_ms;
    TEMP_SENSOR_SELECT temp_sensor_option;
    int32_t  task_priority;
} i2c_devices_daq_task_config_t;

/**
 * @brief Initialize the i2c_devices component
 * @return esp_err_t ESP_OK if successful or an error code
 */
esp_err_t i2c_devices_init();

/**
 * @brief Spawns a Data Acquisition task that periodically reads the data from the I2C connected devices, and writes to LCD every second
 * @note This functions creates the out_data's mutex
 * @param config Config struct
 * @param out_data Read data will be stored into this pointer / struct
 * @return esp_err_t ESP_OK if successful or an error code
 */
esp_err_t i2c_devices_lcd_spawn_daq_task(i2c_devices_daq_task_config_t * config, i2c_devices_data_t * out_data);

/**
 * @brief Force temperatures reading measurment
 * @param out_temp_C [out] Current read temperature
 * @param temp_sensor_option Sensor select
 * @return esp_err_t ESP_OK if successful or an error code
 */
esp_err_t i2c_devices_get_temp(float * out_temp_C, TEMP_SENSOR_SELECT temp_sensor_option);
/**
 * @brief Force humidity reading measurment
 * @param out_humd_P [out] Current read humidity
 * @return esp_err_t ESP_OK if successful or an error code
 */
esp_err_t i2c_devices_get_humd(float * out_humd_P); 
/**
 * @brief Force pressure measurment
 * @param out_pres_Pa [out] Current read pressure
 * @return esp_err_t ESP_OK if successful or an error code
 */
esp_err_t i2c_devices_get_pres(float * out_pres_Pa);

/**
 * @brief Initiates the LCD driver
 * 
 * @return esp_err_t ESP_OK if successful or error code
 */
esp_err_t LCD_init();

/**
 * @brief Switches the LCD's screen to the main screen
 * @param uptime_datetime Pointer to the struct with uptime/datetime information
 * @return esp_err_t ESP_OK or error code
 */
esp_err_t LCD_switch_screen_main(uptime_datetime_t * uptime_datetime);
/**
 * @brief Switches the LCD's screen to the error screen
 * @param error_msg Error message
 * @return esp_err_t ESP_OK or error code
 */
esp_err_t LCD_switch_screen_error(char error_msg[3][20]);

/**
 * @brief Changes the LCD's cursor position
 * @return esp_err_t ESP_OK or error
 */
esp_err_t LCD_gotoxy(uint8_t col, uint8_t row);
/**
 * @brief Writes the given string to the LCD
 * @return esp_err_t ESP_OK or error
 */
esp_err_t LCD_write(char * str);
/**
 * @brief Clears the LCD's screen
 * @return esp_err_t ESP_OK or error
 */
esp_err_t LCD_clear();

#endif