#include "server.h"
#include "cJSON.h"

#define TAG "RESTFUL: DATA"

data_aggregate_t * restful_data_data = NULL;

esp_err_t restful_current_sensors_handler(httpd_req_t * req)
{
    esp_err_t result = ESP_OK;

    if (restful_data_data == NULL) {
        ESP_LOGE(TAG, "restful_data_data WAS NOT SET!");
        return httpd_resp_send_500(req);
    }

    result = httpd_resp_set_type(req, "application/json");
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set request's reponse type");
        return httpd_resp_send_500(req);
    }

    if (xSemaphoreTake(restful_data_data->i2c_daq_data->mutex, portMAX_DELAY) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to acquire data's mutex");
        return httpd_resp_send_500(req);
    }
    float temp = restful_data_data->i2c_daq_data->temperature_C;
    float humd = restful_data_data->i2c_daq_data->humidity_P;
    float pres = restful_data_data->i2c_daq_data->pressure_Pa;
    (void) xSemaphoreGive(restful_data_data->i2c_daq_data->mutex);

    cJSON * object = cJSON_CreateObject();
    if (object == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for cJSON object");
        return httpd_resp_send_500(req);
    }

    (void) cJSON_AddNumberToObject(object, "temperature_C", temp);
    (void) cJSON_AddNumberToObject(object, "humidity_P",    humd);
    (void) cJSON_AddNumberToObject(object, "pressure_Pa",   pres);
    
    char const * json_str = cJSON_Print(object);
    if (json_str == NULL) {
        ESP_LOGE(TAG, "Failed to print cJSON object into a string");
        return httpd_resp_send_500(req);
    }

    result = httpd_resp_send(req, json_str, HTTPD_RESP_USE_STRLEN);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "CURRENT_SENSOR: Failed to send data");
        return httpd_resp_send_500(req);
    }

    free((void *)json_str);
    cJSON_Delete(object);

    return ESP_OK;
}

httpd_uri_t restful_data_current_sensors = {
    .uri     = "/api/data/current_sensors",
    .method  = HTTP_GET,
    .handler = restful_current_sensors_handler,
};

esp_err_t restful_settings_handler(httpd_req_t * req)
{
    esp_err_t result = ESP_OK;

    if (restful_data_data == NULL) {
        ESP_LOGE(TAG, "restful_data_data WAS NOT SET!");
        return httpd_resp_send_500(req);
    }

    result = httpd_resp_set_type(req, "application/json");
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set request's reponse type");
        return httpd_resp_send_500(req);
    }

    cJSON * object = cJSON_CreateObject();
    if (object == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for the cJSON object");
        return httpd_resp_send_500(req);
    }

    if (xSemaphoreTake(restful_data_data->user_settings->mutex, portMAX_DELAY) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to acquire data's mutex");
        return httpd_resp_send_500(req);
    }
    (void) cJSON_AddStringToObject(object, "wifi_name", restful_data_data->user_settings->wifi_name);
    (void) cJSON_AddStringToObject(object, "wifi_password", restful_data_data->user_settings->wifi_password);
    (void) cJSON_AddStringToObject(object, "mdns_hostname", restful_data_data->user_settings->mdns_hostname);
    (void) cJSON_AddStringToObject(object, "tz_code", restful_data_data->user_settings->tz_code);
    (void) cJSON_AddNumberToObject(object, "serial_logging_verbosity", restful_data_data->user_settings->serial_logging_verbosity);
    (void) cJSON_AddNumberToObject(object, "wifi_rssi_test_period_ms", restful_data_data->user_settings->wifi_rssi_test_period_ms);
    (void) cJSON_AddNumberToObject(object, "i2c_devices_daq_interval_ms", restful_data_data->user_settings->i2c_devices_daq_interval_ms);
    (void) cJSON_AddNumberToObject(object, "i2c_devices_daq_temp_sensor_select", restful_data_data->user_settings->i2c_devices_daq_temp_sensor_select);
    (void) cJSON_AddNumberToObject(object, "data_write_to_sdcard_interval", restful_data_data->user_settings->data_write_to_sdcard_interval);
    (void) xSemaphoreGive(restful_data_data->user_settings->mutex);

    char const * json_str = cJSON_Print(object);
    if (json_str == NULL) {
        ESP_LOGE(TAG, "Failed to print cJSON object into string");
        return httpd_resp_send_500(req);
    }

    result = httpd_resp_send(req, json_str, HTTPD_RESP_USE_STRLEN);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "SETTINGS_GET: Failed to send data");
        return httpd_resp_send_500(req);
    }

    free((void *)json_str);
    cJSON_Delete(object);

    return ESP_OK;
}

httpd_uri_t restful_data_settings = {
    .uri     = "/api/data/settings",
    .method  = HTTP_GET,
    .handler = restful_settings_handler,
};
