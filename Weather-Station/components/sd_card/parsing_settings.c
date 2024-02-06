#include "sd_card.h"

// This whole file is a mess
// Sorry for anyone that has to read this lmao

#define TAG "SD_CARD_COMPONENT"
#define BUFFER_LENGTH 128
#define FILE_CONTENTS_MAX_LEN 2048


esp_err_t sd_card_read_settings(basic_settings_t * /*out*/settings)
{
    esp_err_t result = ESP_OK;

    if (settings == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    free_settings(settings);

    char settings_path[] = "/sdcard/settings.txt";

    FILE * file = fopen(settings_path, "r");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open the settings file: %s", settings_path);
        return ESP_ERR_NOT_FOUND;
    }

    char buffer[BUFFER_LENGTH] = {0};
    char file_contents[FILE_CONTENTS_MAX_LEN] = {0};
    size_t current_len = 0;
    size_t line_len = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        line_len = strlen(buffer);

        if (current_len + line_len >= FILE_CONTENTS_MAX_LEN) {
            ESP_LOGE(TAG, "The read settings.txt file's size exceeded maximum allowed size of %d Bytes", FILE_CONTENTS_MAX_LEN);
            return ESP_ERR_NO_MEM;
        }

        (void) strncpy(file_contents + current_len, buffer, line_len);
        current_len += line_len;

        (void) memset(buffer, 0, sizeof(buffer));
    }

    (void) fclose(file);

    // printf("%s", file_contents);

    cJSON * json_settings = cJSON_Parse(file_contents);
    if (json_settings == NULL) {
        ESP_LOGE(TAG, "Failed to parse the settings.txt data");
        char const * error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            ESP_LOGE(TAG, "cJSON error before: %s", error_ptr);
        }
        result = ESP_FAIL;
        goto exit_label;
    }

    (void) xSemaphoreTake(settings->mutex, portMAX_DELAY);

    settings->was_generated_by_default = false;

    cJSON const * json_object = NULL;
    size_t len = 0;

    // Get the wifi_name value
    json_object = cJSON_GetObjectItemCaseSensitive(json_settings, "wifi_name");
    if ((cJSON_IsString(json_object) == false) || (json_object->valuestring == NULL)) {
        ESP_LOGE(TAG, "in settings.txt's data: wifi_name is not a string or is NULL");
        result = ESP_ERR_INVALID_ARG;
        goto exit_label;
    }
    len = strlen(json_object->valuestring) + 1;
    if (len > 31) {
        ESP_LOGE(TAG, "The wifi_name settings is longer than 31 characters. Can not connect to such network");
        result = ESP_ERR_INVALID_ARG;
        goto exit_label;
    }
    settings->wifi_name = (char *)malloc(sizeof(char) * len);
    if (settings->wifi_name == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for basic_settings_t' field wifi_name");
        result = ESP_ERR_NO_MEM;
        goto exit_label;
    }
    (void) memset(settings->wifi_name, 0, len);
    (void) strncpy(settings->wifi_name, json_object->valuestring, len - 1);

    // Get the wifi_password value
    json_object = cJSON_GetObjectItemCaseSensitive(json_settings, "wifi_password");
    if ((cJSON_IsString(json_object) == false) || (json_object->valuestring == NULL)) {
        ESP_LOGE(TAG, "in settings.txt's data: wifi_password is not a string or is NULL");
        result = ESP_ERR_INVALID_ARG;
        goto exit_label;
    }
    len = strlen(json_object->valuestring) + 1;
    if (len > 63) {
        ESP_LOGE(TAG, "The wifi_password settings is longer than 63 characters. Can not connect to such network");
        result = ESP_ERR_INVALID_ARG;
        goto exit_label;
    }
    settings->wifi_password = (char *)malloc(sizeof(char) * len);
    if (settings->wifi_password == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for basic_settings_t' field wifi_password");
        result = ESP_ERR_NO_MEM;
        goto exit_label;
    }
    (void) memset(settings->wifi_password, 0, len);
    (void) strncpy(settings->wifi_password, json_object->valuestring, len - 1);

    // Get the mdns_hostname value
    json_object = cJSON_GetObjectItemCaseSensitive(json_settings, "mdns_hostname");
    if ((cJSON_IsString(json_object) == false) || (json_object->valuestring == NULL)) {
        ESP_LOGE(TAG, "in settings.txt's data: mdns_hostname is not a string or is NULL");
        result = ESP_ERR_INVALID_ARG;
        goto exit_label;
    }
    len = strlen(json_object->valuestring) + 1;
    settings->mdns_hostname = (char *)malloc(sizeof(char) * len);
    if (settings->mdns_hostname == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for basic_settings_t' field mdns_hostname");
        result = ESP_ERR_NO_MEM;
        goto exit_label;
    }
    (void) memset(settings->mdns_hostname, 0, len);
    (void) strncpy(settings->mdns_hostname, json_object->valuestring, len - 1);

    // Get the tz_code value
    json_object = cJSON_GetObjectItemCaseSensitive(json_settings, "tz_code");
    if ((cJSON_IsString(json_object) == false) || (json_object->valuestring == NULL)) {
        ESP_LOGE(TAG, "in settings.txt's data: tz_code is not a string or is NULL");
        result = ESP_ERR_INVALID_ARG;
        goto exit_label;
    }
    len = strlen(json_object->valuestring) + 1;
    settings->tz_code = (char *)malloc(sizeof(char) * len);
    if (settings->tz_code == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for basic_settings_t' field tz_code");
        result = ESP_ERR_NO_MEM;
        goto exit_label;
    }
    (void) memset(settings->tz_code, 0, len);
    (void) strncpy(settings->tz_code, json_object->valuestring, len - 1);

    // Get the serial_logging_verbosity value
    json_object = cJSON_GetObjectItemCaseSensitive(json_settings, "serial_logging_verbosity");
    if (cJSON_IsNumber(json_object) == false) {
        ESP_LOGE(TAG, "in settings.txt's data: serial_logging_verbosity is not a number or is NULL");
        result = ESP_ERR_INVALID_ARG;
        goto exit_label;
    }
    settings->serial_logging_verbosity = json_object->valueint;

    // Get the wifi_rssi_test_period_ms value
    json_object = cJSON_GetObjectItemCaseSensitive(json_settings, "wifi_rssi_test_period_ms");
    if (cJSON_IsNumber(json_object) == false) {
        ESP_LOGE(TAG, "in settings.txt's data: wifi_rssi_test_period_ms is not a number or is NULL");
        result = ESP_ERR_INVALID_ARG;
        goto exit_label;
    }
    settings->wifi_rssi_test_period_ms = json_object->valueint;

    // Get the i2c_devices_daq_interval_ms value
    json_object = cJSON_GetObjectItemCaseSensitive(json_settings, "i2c_devices_daq_interval_ms");
    if (cJSON_IsNumber(json_object) == false) {
        ESP_LOGE(TAG, "in settings.txt's data: i2c_devices_daq_interval_ms is not a number or is NULL");
        result = ESP_ERR_INVALID_ARG;
        goto exit_label;
    }
    settings->i2c_devices_daq_interval_ms = json_object->valueint;

    // Get the i2c_devices_daq_temp_sensor_select value
    json_object = cJSON_GetObjectItemCaseSensitive(json_settings, "i2c_devices_daq_temp_sensor_select");
    if (cJSON_IsNumber(json_object) == false) {
        ESP_LOGE(TAG, "in settings.txt's data: i2c_devices_daq_temp_sensor_select is not a number or is NULL");
        result = ESP_ERR_INVALID_ARG;
        goto exit_label;
    }
    settings->i2c_devices_daq_temp_sensor_select = json_object->valueint;

    // Get the data_write_to_sdcard_interval value
    json_object = cJSON_GetObjectItemCaseSensitive(json_settings, "data_write_to_sdcard_interval");
    if (cJSON_IsNumber(json_object) == false) {
        ESP_LOGE(TAG, "in settings.txt's data: data_write_to_sdcard_interval is not a number or is NULL");
        result = ESP_ERR_INVALID_ARG;
        goto exit_label;
    }
    settings->data_write_to_sdcard_interval = json_object->valueint;

    exit_label:
    (void) xSemaphoreGive(settings->mutex);
    cJSON_Delete(json_settings);
    return result;
}

esp_err_t sd_card_write_settings(basic_settings_t const * /*in*/settings)
{
    if (settings == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t result = ESP_OK;
    char * str = NULL;

    cJSON * json_settings = cJSON_CreateObject();
    if (json_settings == NULL) {
        ESP_LOGE(TAG, "Could not create cJSON object");
        return ESP_FAIL;
    }

    cJSON * json_object = NULL;

    (void) xSemaphoreTake(settings->mutex, portMAX_DELAY);
    json_object = cJSON_CreateString(settings->wifi_name);
    (void) xSemaphoreGive(settings->mutex);
    if (json_object == NULL) {
        ESP_LOGE(TAG, "Could not create cJSON string \"wifi_name\"");
        result = ESP_FAIL;
        goto exit_label;
    }
    cJSON_AddItemToObject(json_settings, "wifi_name", json_object);

    (void) xSemaphoreTake(settings->mutex, portMAX_DELAY);
    json_object = cJSON_CreateString(settings->wifi_password);
    (void) xSemaphoreGive(settings->mutex);
    if (json_object == NULL) {
        ESP_LOGE(TAG, "Could not create cJSON string \"wifi_password\"");
        result = ESP_FAIL;
        goto exit_label;
    }
    cJSON_AddItemToObject(json_settings, "wifi_password", json_object);

    (void) xSemaphoreTake(settings->mutex, portMAX_DELAY);
    json_object = cJSON_CreateString(settings->mdns_hostname);
    (void) xSemaphoreGive(settings->mutex);
    if (json_object == NULL) {
        ESP_LOGE(TAG, "Could not create cJSON string \"mdns_hostname\"");
        result = ESP_FAIL;
        goto exit_label;
    }
    cJSON_AddItemToObject(json_settings, "mdns_hostname", json_object);

    (void) xSemaphoreTake(settings->mutex, portMAX_DELAY);
    json_object = cJSON_CreateString(settings->tz_code);
    (void) xSemaphoreGive(settings->mutex);
    if (json_object == NULL) {
        ESP_LOGE(TAG, "Could not create cJSON string \"tz_code\"");
        result = ESP_FAIL;
        goto exit_label;
    }
    cJSON_AddItemToObject(json_settings, "tz_code", json_object);

    (void) xSemaphoreTake(settings->mutex, portMAX_DELAY);
    json_object = cJSON_CreateNumber(settings->serial_logging_verbosity);
    (void) xSemaphoreGive(settings->mutex);
    if (json_object == NULL) {
        ESP_LOGE(TAG, "Could not create cJSON number serial_logging_verbosity");
        result = ESP_FAIL;
        goto exit_label;
    }
    cJSON_AddItemToObject(json_settings, "serial_logging_verbosity", json_object);

    (void) xSemaphoreTake(settings->mutex, portMAX_DELAY);
    json_object = cJSON_CreateNumber(settings->wifi_rssi_test_period_ms);
    (void) xSemaphoreGive(settings->mutex);
    if (json_object == NULL) {
        ESP_LOGE(TAG, "Could not create cJSON number wifi_rssi_test_period_ms");
        result = ESP_FAIL;
        goto exit_label;
    }
    cJSON_AddItemToObject(json_settings, "wifi_rssi_test_period_ms", json_object);

    (void) xSemaphoreTake(settings->mutex, portMAX_DELAY);
    json_object = cJSON_CreateNumber(settings->i2c_devices_daq_interval_ms);
    (void) xSemaphoreGive(settings->mutex);
    if (json_object == NULL) {
        ESP_LOGE(TAG, "Could not create cJSON number i2c_devices_daq_interval_ms");
        result = ESP_FAIL;
        goto exit_label;
    }
    cJSON_AddItemToObject(json_settings, "i2c_devices_daq_interval_ms", json_object);

    (void) xSemaphoreTake(settings->mutex, portMAX_DELAY);
    json_object = cJSON_CreateNumber(settings->i2c_devices_daq_temp_sensor_select);
    (void) xSemaphoreGive(settings->mutex);
    if (json_object == NULL) {
        ESP_LOGE(TAG, "Could not create cJSON number i2c_devices_daq_temp_sensor_select");
        result = ESP_FAIL;
        goto exit_label;
    }
    cJSON_AddItemToObject(json_settings, "i2c_devices_daq_temp_sensor_select", json_object);

    (void) xSemaphoreTake(settings->mutex, portMAX_DELAY);
    json_object = cJSON_CreateNumber(settings->data_write_to_sdcard_interval);
    (void) xSemaphoreGive(settings->mutex);
    if (json_object == NULL) {
        ESP_LOGE(TAG, "Could not create cJSON number data_write_to_sdcard_interval");
        result = ESP_FAIL;
        goto exit_label;
    }
    cJSON_AddItemToObject(json_settings, "data_write_to_sdcard_interval", json_object);

    str = cJSON_Print(json_settings);
    if (str == NULL) {
        ESP_LOGE(TAG, "Failed to print cJSON object tree to string");
        result = ESP_ERR_NO_MEM;
        goto exit_label;
    }

    result = sd_card_write("settings.txt", str, OVERWRITE);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write the settings string to the settings file");
        goto exit_label;
    }

    exit_label:
    cJSON_Delete(json_settings);
    if (str != NULL) {
        free(str);
    }

    return ESP_OK;
}
