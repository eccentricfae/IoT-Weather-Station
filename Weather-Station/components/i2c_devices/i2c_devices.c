#include "i2c_devices.h"

#define TAG "I2C_DEVICES"

typedef struct {
    i2c_devices_daq_task_config_t * config;
    i2c_devices_data_t * out_data;
} daq_task_data_t;


i2c_dev_t am2320_dev = {0};
bmp280_t bmp280_dev  = {0};

static esp_err_t am2320_read_temp_humd(float * out_temp, float * out_humd);
static esp_err_t bmp280_read_press_temp(float * out_press, float * out_temp);
void daq_task_entry(void * args);

#define MUTEX_MS_TO_WAIT 500

static i2c_dev_t pcf8574 = {0};
static hd44780_t lcd = {0};
bool skip_lcd_writing = true;
static char error_msg[3][20] = {0};
static uptime_datetime_t  * uptime = NULL;


esp_err_t i2c_devices_init()
{
    esp_err_t result = ESP_OK;

    result = i2cdev_init();
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to inic i2cdev");
        return ESP_FAIL;
    }

    result = am2320_init_desc(&am2320_dev, 0, I2C_SDA_PIN, I2C_SCL_PIN);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init the AM2320 device");
        return ESP_FAIL;
    }

    static bmp280_params_t bmp280_params;
    result = bmp280_init_default_params(&bmp280_params);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize default BMP280 paramteres");
        return result;
    }

    bmp280_params.mode     = BMP280_MODE_NORMAL;
    bmp280_params.filter   = BMP280_FILTER_16;
    bmp280_params.oversampling_pressure    = BMP280_STANDARD;
    bmp280_params.oversampling_temperature = BMP280_STANDARD;
    bmp280_params.standby  = BMP280_STANDBY_1000;
    
    result = bmp280_init_desc(&bmp280_dev, BMP280_I2C_ADDRESS_0, 0, I2C_SDA_PIN, I2C_SCL_PIN);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize BMP280 device descriptor");
        return result;
    }

    result = bmp280_init(&bmp280_dev, &bmp280_params);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize the BMP280 device");
        return result;
    }

    return ESP_OK;
}

esp_err_t i2c_devices_lcd_spawn_daq_task(i2c_devices_daq_task_config_t * config, i2c_devices_data_t * out_data)
{
    if (config == NULL) {
        ESP_LOGE(TAG, "Bad config pointer");
        ESP_LOGE(TAG, "Passed NULL pointer to the i2c_devices_daq_task_config_t struct to the i2c_devices_spawn_daq_task() function");
        return ESP_ERR_INVALID_ARG;
    }

    if (out_data == NULL) {
        ESP_LOGE(TAG, "Passed NULL pointer to the i2c_devices_data_t struct to the i2c_devices_spawn_daq_task() function");
        return ESP_ERR_INVALID_ARG;
    }

    out_data->mutex = xSemaphoreCreateMutex();
    if (out_data->mutex == NULL) {
        ESP_LOGE(TAG, "Failed to create i2c_devices_data_t's mutex");
        return ESP_FAIL;
    }

    static daq_task_data_t daq_data = {0};
    daq_data.config   = config;
    daq_data.out_data = out_data;

    BaseType_t task_result = xTaskCreate(
        daq_task_entry,
        "I2C DEVICES DATA ACQUISITION TASK",
        3072,
        &daq_data,
        config->task_priority,
        NULL
    );

    if (task_result != pdPASS) {
        ESP_LOGE(TAG, "Failed to spawn the I2C DEVICES DATA ACQUISITION TASK task");
        return ESP_ERR_INVALID_RESPONSE;
    }

    return ESP_OK;
}

esp_err_t i2c_devices_get_temp(float * out_temp_C, TEMP_SENSOR_SELECT temp_sensor_option)
{
    esp_err_t result = ESP_OK;
    float temp = 0.0f;
    float humd = 0.0f;
    float pressure = 0.0f;

    switch (temp_sensor_option) {
        case TEMP_SENSOR_AM2320:
            result = am2320_read_temp_humd(&temp, &humd);
            if (result ==  ESP_OK)
                ESP_LOGD(TAG, "AM2320 Temperature: %.1f C, Humidity %.1f %%\n", temp, humd);
            break;

        case TEMP_SENSOR_BMP280:
            result = bmp280_read_press_temp(&pressure, &temp);
            if (result ==  ESP_OK)
                ESP_LOGD(TAG, "BMP280 Temperature: %.1f C, Pressure %.1f PA\n", temp, pressure);
            break;

        case TEMP_SENSOR_MEAN:
        default:
            float t1 = 0.0f;
            float t2 = 0.0f;
            result = am2320_read_temp_humd(&t1, &humd);
            result |= bmp280_read_press_temp(&pressure, &t2);
            temp = (t1 + t2) / 2.0f;
            if (result ==  ESP_OK) {
                ESP_LOGD(TAG, "AM2320 Temperature: %.1f C, Humidity %.1f %%\n", t1, humd);
                ESP_LOGD(TAG, "BMP280 Temperature: %.1f C, Pressure %.1f PA\n", t2, pressure);
            }
            break;
    }

    if (result != ESP_OK) {
        return result;
    }

    *out_temp_C = temp;

    return ESP_OK;
}

esp_err_t i2c_devices_get_humd(float * out_humd_P)
{
    esp_err_t result = ESP_OK;

    float temp = 0.0f;
    float humd = 0.0f;

    result = am2320_read_temp_humd(&temp, &humd);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get data from the AM2320 device");
        return result;
    }

    *out_humd_P = humd;

    return ESP_OK;
}

esp_err_t i2c_devices_get_pres(float * out_pres_Pa)
{
    esp_err_t result = ESP_OK;

    float p = 0.0f;
    float t = 0.0f;

    result = bmp280_read_press_temp(&p, &t);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get data from the BMP280 device");
        return result;
    }

    *out_pres_Pa = p;

    return ESP_OK;
}

static esp_err_t am2320_read_temp_humd(float * out_temp, float * out_humd)
{
    float t, h;
    esp_err_t result = am2320_get_rht(&am2320_dev, &t, &h);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get the data from the AM2320 device");
        return result;
    }

    *out_temp = t;
    *out_humd = h;

    return ESP_OK;
}

static esp_err_t bmp280_read_press_temp(float * out_press, float * out_temp)
{
    esp_err_t result = ESP_OK;

    float t = 0.0f;
    float p = 0.0f;

    result = bmp280_read_float(&bmp280_dev, &t, &p, NULL);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read data from the BMP280 device");
        return result;
    }

    *out_press = p;
    *out_temp  = t;

    return ESP_OK;
}

void daq_task_entry(void * args)
{
    daq_task_data_t * task_data = (daq_task_data_t *)args;
    esp_err_t result = ESP_OK;
    float temp_am2320 = 0.0f;
    float temp_bmp280 = 0.0f;
    float temp  = 0.0f;
    float humd  = 0.0f;
    float press = 0.0f;
    uint32_t interval = 1000; // Default value 1s

    // LCD data
    char str[20] = {0};
    uint32_t execs_per_1sec = 1000 / interval;
    uint32_t daq_cntr = execs_per_1sec; // Make sure it runs on first loop

    while (true) {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // I2C DAQ Portion
        daq_cntr++;

        result = am2320_read_temp_humd(&temp_am2320, &humd);
        if (result != ESP_OK) {
            goto daq_failed;
        }

        result = bmp280_read_press_temp(&press, &temp_bmp280);
        if (result != ESP_OK) {
            goto daq_failed;
        }

        switch (task_data->config->temp_sensor_option) {
            case TEMP_SENSOR_AM2320:
                temp = temp_am2320;
                break;

            case TEMP_SENSOR_BMP280:
                temp = temp_bmp280;
                break;

            case TEMP_SENSOR_MEAN:
            default:
                temp = (temp_am2320 + temp_bmp280) / 2;
                break;
        }

        if (xSemaphoreTake(task_data->out_data->mutex, pdMS_TO_TICKS(interval)) == pdFALSE) {
            // If the function will wait for the amount of time that is equal to the daq interval
            // it should abort writing current data, get new data and try again to write it
            ESP_LOGE(TAG, "DAQ TASK: Timed out waiting for data's mutex");
            continue;
        }
        task_data->out_data->temperature_C = temp;
        task_data->out_data->humidity_P    = humd;
        task_data->out_data->pressure_Pa   = press;
        interval = task_data->config->daq_interval_ms;
        execs_per_1sec = 1000 / interval;
        if (interval < 100) {
            // For some unknown and ungodly reason the interval of this loop cannot be less than 100 ms
            // Tried debugging this weird issue for hours :(            
            interval = 100;
        }
        (void) xSemaphoreGive(task_data->out_data->mutex);
        
        daq_failed:
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // LCD Portion

        if (skip_lcd_writing) {
            goto cont;
        }

        // Check if more than 1 sec has passed using software counters
        bool update_lcd = daq_cntr >= execs_per_1sec;
        if (update_lcd == false) {
            goto cont;
        }
        daq_cntr = 0; // Clear the counter

        memset(str, 0, sizeof(str));

        if (xSemaphoreTake(uptime->mutex, pdMS_TO_TICKS(MUTEX_MS_TO_WAIT)) != pdTRUE) {
            goto skip_lcd;
        }

        if (   (uptime->date_time.tm_year != 70) 
            && (uptime->date_time.tm_year != 69)) {
            (void) strftime(str, 20, "%R   %d/%m/%y", &(uptime->date_time));
            (void) LCD_gotoxy(2, 0);
            (void) LCD_write(str);
        }
        (void) xSemaphoreGive(uptime->mutex);
        
        skip_lcd:
        (void) sprintf(str, "%.1f  C", temp);
        (void) LCD_gotoxy(13, 1);
        (void) LCD_write(str);
        (void) LCD_gotoxy(18, 1);
        (void) LCD_write("\xDF");

        (void) sprintf(str, "%.1f %%", humd);
        (void) LCD_gotoxy(13, 2);
        (void) LCD_write(str);

        int pres_to_disp = (int)press / 100;
        int tmp_col = pres_to_disp >= 1000 ? 12 : 13;
        (void) sprintf(str, "%d hPa", pres_to_disp);
        (void) LCD_gotoxy(tmp_col, 3);
        (void) LCD_write(str);

        cont:
        vTaskDelay(pdMS_TO_TICKS(interval));
    }
}

esp_err_t write_lcd_data(hd44780_t const * lcd, uint8_t data)
{
    return pcf8574_port_write(&pcf8574, data);
}

esp_err_t LCD_init()
{
    esp_err_t result = ESP_OK;

    hd44780_t lcd_1 = {
        .write_cb = write_lcd_data, // Callback for writing data using I2C rather than normal HD44780 interface
        .font = HD44780_FONT_5X8,
        .lines = 4,                 // Number of display lines
        .pins = {
            // Do not delete :)
            .rs = 0,
            .e  = 2,
            .d4 = 4,
            .d5 = 5,
            .d6 = 6,
            .d7 = 7,
            .bl = 3
        }
    };

    memcpy(&lcd, &lcd_1, sizeof(hd44780_t));

    result = pcf8574_init_desc(&pcf8574, 0x27, 0, I2C_SDA_PIN, I2C_SCL_PIN);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize the pcf8574");
        return result;
    }

    result = hd44780_init(&lcd);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize the hd44780");
        return result;
    }

    result = hd44780_switch_backlight(&lcd, true);
    result |= hd44780_clear(&lcd);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to properly turn on the LCD");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t LCD_switch_screen_main(uptime_datetime_t * uptime_datetime)
{
    if (uptime_datetime == NULL) {
        ESP_LOGE(TAG, "Bad argumetns passed to the LCD_switch_screen_main() function!");
        return ESP_FAIL;
    }

    uptime = uptime_datetime;

    esp_err_t result = ESP_OK;
    result = hd44780_clear(&lcd);
    result |= LCD_gotoxy(0, 1);
    result |= LCD_write("Temperature:");
    result |= LCD_gotoxy(0, 2);
    result |= LCD_write("Humidity:");
    result |= LCD_gotoxy(0, 3);
    result |= LCD_write("Pressure:");

    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to properly switch to the MAIN SCREEN on the LCD");
        return ESP_FAIL;
    }

    skip_lcd_writing = false;

    return ESP_OK;
}

esp_err_t LCD_switch_screen_error(char l_error_msg[3][20])
{
    if (l_error_msg == NULL) {
        ESP_LOGE(TAG, "Passed NULL pointer as error_msg to the LCD_switch_screen_error() function");
        return ESP_FAIL;
    }

    // Copy the contents in case they'll disappear
    strncpy(error_msg[0], l_error_msg[0], 20);
    strncpy(error_msg[1], l_error_msg[1], 20);
    strncpy(error_msg[2], l_error_msg[2], 20);

    esp_err_t result = ESP_OK;
    result |= hd44780_clear(&lcd);
    result |= LCD_gotoxy(0, 0);
    result |= LCD_write("ERROR:");
    result |= LCD_gotoxy(0, 1);
    result |= LCD_write(error_msg[0]);
    result |= LCD_gotoxy(0, 2);
    result |= LCD_write(error_msg[1]);
    result |= LCD_gotoxy(0, 3);
    result |= LCD_write(error_msg[2]);
    
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to properly switch to the Error Screen on the LCD");
        return ESP_FAIL;
    }

    skip_lcd_writing = true;

    return ESP_OK;
}

esp_err_t LCD_gotoxy(uint8_t col, uint8_t row)
{
    esp_err_t result = hd44780_gotoxy(&lcd, col, row);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "LCD_gotoxy() Failed to move the cursor");
        return result;
    }

    return ESP_OK;
}

esp_err_t LCD_write(char * str)
{
    if (strlen(str) > 20) {
        ESP_LOGW(TAG, "Tried to write a string longer than 20 characters to the LCD, where one line is only 20 charaters long");
    }

    esp_err_t result = hd44780_puts(&lcd, str);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "LCD_write failed");
        return result;
    }

    return ESP_OK;
}

esp_err_t LCD_clear()
{
    esp_err_t result = hd44780_clear(&lcd);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to clear the lcd");
        return result;
    }

    return ESP_OK;
}
