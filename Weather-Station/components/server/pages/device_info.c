#include "server.h"

char const device_info_data[] = 
	"<!DOCTYPE html>\n"
	"<html lang='en'>\n"
	"<head>\n"
	"    <meta charset='UTF-8'>\n"
	"    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
	"    <title>Weather Station | Device Info</title>\n"
	"    <style>\n"
	"        body {\n"
	"            font-family: Arial, sans-serif;\n"
	"            margin: 20px;\n"
	"        }\n"
	"\n"
	"        header {\n"
	"            display: flex;\n"
	"            justify-content: center;\n"
	"            align-items: center;\n"
	"            padding: 10px;\n"
	"            background-color: #7d34b2;\n"
	"            color: #fff;\n"
	"            border-radius: 10px;\n"
	"        }\n"
	"\n"
	"        header > * {\n"
	"            margin-top: 5px;\n"
	"            margin-bottom: 5px;\n"
	"        }\n"
	"\n"
	"        @media screen and (min-width: 480px) {\n"
	"            .header-buttons {\n"
	"                display: flex;\n"
	"                flex-direction: row;\n"
	"                justify-content: space-around;\n"
	"                align-items: center;\n"
	"                flex-wrap: wrap;\n"
	"                gap: 10px;\n"
	"            }\n"
	"        }\n"
	"\n"
	"        @media screen and (max-width: 480px) {\n"
	"            .header-buttons {\n"
	"                display: flex;\n"
	"                flex-direction: column;\n"
	"                justify-content: space-around;\n"
	"                align-items: center;\n"
	"                flex-wrap: wrap;\n"
	"                gap: 10px;\n"
	"            }\n"
	"        }\n"
	"\n"
	"        .header-button {\n"
	"            background-color: #fff;\n"
	"            color: #7d34b2;\n"
	"            border: none;\n"
	"            padding: 5px 10px;\n"
	"            border-radius: 5px;\n"
	"            cursor: pointer;\n"
	"            margin-left: 20px;\n"
	"            margin-right: 20px;\n"
	"            font-size: 20px;\n"
	"            text-decoration:none;\n"
	"        }\n"
	"\n"
	"        .header-button:hover {\n"
	"            box-shadow: 0px 0px 20px yellow;\n"
	"        }\n"
	"\n"
	"        #body_title > h1{\n"
	"            text-align: center;\n"
	"            margin: auto;\n"
	"            background-color: white;\n"
	"            color: #7d34b2;\n"
	"            padding: 10px;\n"
	"            width: 250px;\n"
	"            border: 2px solid #7d34b2;\n"
	"            border-radius: 20px;\n"
	"            margin-bottom: 20px;\n"
	"        }\n"
	"\n"
	"        section {\n"
	"            margin-top: 20px;\n"
	"        }\n"
	"\n"
	"        @media screen and (max-width: 480px) {\n"
	"            section {\n"
	"                margin-left: -10px;\n"
	"            }\n"
	"        }\n"
	"\n"
	"        .info_div {\n"
	"            display: flex;\n"
	"            justify-content: space-around;\n"
	"            align-items: center;\n"
	"            flex-wrap: wrap;\n"
	"            flex-direction: column;\n"
	"        }\n"
	"\n"
	"        .info_mid {\n"
	"            display: flex;\n"
	"            justify-content: space-around;\n"
	"            align-items: center;\n"
	"            flex-wrap: wrap;\n"
	"        }\n"
	"\n"
	"        .info_card {\n"
	"            width: 300px;\n"
	"            height: 150px;\n"
	"            padding: 10px;\n"
	"            margin: 10px;\n"
	"            border: 1px solid #ddd;\n"
	"            border-radius: 8px;\n"
	"            box-shadow: 0 0 10px #7d34b2;\n"
	"        }\n"
	"\n"
	"        .info_card > h2, p {\n"
	"            text-align: center;\n"
	"        }\n"
	"    </style>\n"
	"</head>\n"
	"<body>\n"
	"    <header>\n"
	"        <div class='header-buttons'>\n"
	"            <a class='header-button' href='./'>Home</a>\n"
	"            <a class='header-button' href='./device_info'>Device Info</a>\n"
	"            <a class='header-button' href='./settings'>Settings</a>\n"
	"        </div>\n"
	"    </header>\n"
	"\n"
	"    <section>\n"
	"        <div id='body_title'>\n"
	"            <h1>Weather Station's Device Info</h1>\n"
	"        </div>\n"
	"\n"
	"        <div class='info_div'>\n"
	"            <div class='info_mid'>\n"
	"            <div class='info_card'>\n"
	"                <h2>Signal Strength (RSSI)</h2>\n"
	"                <h2><span id='signal_rssi'>Good (20)</span></h2>\n"
	"            </div>\n"
	"            <div class='info_card'>\n"
	"                <h2>Device's IP</h2>\n"
	"                <h2><span id='ip_addr'>000.000.000.000</span></h2>\n"
	"            </div>\n"
	"            </div>\n"
	"            <div class='info_mid'>\n"
	"            <div class='info_card'>\n"
	"                <h2>Local Time</h2>\n"
	"                <h2><span id='local_time'>23.06.1999 09:15</span></h2>\n"
	"            </div>\n"
	"            <div class='info_card'>\n"
	"                <h2>Device's Uptime</h2>\n"
	"                <h2><span id='uptime'>00d 00h 00m 00s</span></h2>\n"
	"            </div>\n"
	"            <div class='info_card'>\n"
	"                <h2>Set Timezone</h2>\n"
	"                <h2><span id='timezone'>GMT+0</span></h2>\n"
	"            </div>\n"
	"            </div>\n"
	"            <div class='info_mid'>\n"
	"            <div class='info_card'>\n"
	"                <h2>mDNS Hostname</h2>\n"
	"                <h2><span id='mdns_hostname'>esp</span></h2>\n"
	"            </div>\n"
	"            <div class='info_card'>\n"
	"                <h2>Sensor Used For Temperature Measurment</h2>\n"
	"                <h2><span id='temp_sensor'>AM2320 / BMP280 / both</span></h2>\n"
	"            </div>\n"
	"            </div>\n"
	"        </div>\n"
	"    </section>\n"
	"\n"
	"    <script>\n"
;

// !@#$%

#define TAG "device_info.c"

data_aggregate_t const * device_info_page_device_data = NULL;

void rssi_to_text_description(int32_t rssi, char * out_desc)
{
	if (rssi >= -65) {
		strcpy(out_desc, "GREAT");
	} else if (rssi < -65 && rssi >= -75) {
		strcpy(out_desc, "GOOD");
	} else if (rssi < -75 && rssi >= -85) {
		strcpy(out_desc, "FAIR");
	} else if (rssi < -85 && rssi >= -95) {
		strcpy(out_desc, "POOR");
	} else {
		strcpy(out_desc, "TERRIBLE");
	}
}

void uptime_s_to_str(int64_t uptime_s, char * out_str)
{
	int days = uptime_s / 86400;
	uptime_s = uptime_s % 86400;

	int hours = uptime_s / 3600;
	uptime_s = uptime_s % 3600;

	int minutes = uptime_s / 60;
	uptime_s = uptime_s % 60;

	// int seconds = uptime_s;

	sprintf(out_str, "%dd %dh %dm %llds", days, hours, minutes, uptime_s);
}

esp_err_t device_info_handler(httpd_req_t * req)
{
    esp_err_t result = ESP_OK;

    if (device_info_page_device_data == NULL) {
        ESP_LOGE(TAG, "device_info_page_device_data was not set!");
        return httpd_resp_send_500(req);
    }

    result = httpd_resp_send_chunk(req, device_info_data, HTTPD_RESP_USE_STRLEN);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed sending the page's main content chunk");
        return httpd_resp_send_500(req);
    }

    char rssi_as_text[12] = {0};
    char str[256] = {0};

    (void) xSemaphoreTake(device_info_page_device_data->wifi_state->mutex, portMAX_DELAY);
    rssi_to_text_description(device_info_page_device_data->wifi_state->wifi_rssi, rssi_as_text);
    sprintf(
        str, 
		"document.getElementById('signal_rssi').textContent = '%s (%ld)';"
        "document.getElementById('ip_addr').textContent = '%s';",
        rssi_as_text,
        device_info_page_device_data->wifi_state->wifi_rssi,
        device_info_page_device_data->wifi_state->ip
    );
    (void) xSemaphoreGive(device_info_page_device_data->wifi_state->mutex);

    result = httpd_resp_send_chunk(req, str, HTTPD_RESP_USE_STRLEN);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send a chunk of the response");
        return httpd_resp_send_500(req);
    }

    char uptime_str[20]     = {0};
    char local_time_str[23] = {0};

    (void) xSemaphoreTake(device_info_page_device_data->uptime->mutex, portMAX_DELAY);
    strftime(local_time_str, sizeof(local_time_str), "%d/%m/%Y %X", &(device_info_page_device_data->uptime->date_time));
	uptime_s_to_str(device_info_page_device_data->uptime->uptime_s, uptime_str);
    sprintf(
        str, 
        "document.getElementById('local_time').textContent = '%s';"
        "document.getElementById('uptime').textContent = '%s';",
        local_time_str,
        uptime_str
    );
    (void) xSemaphoreGive(device_info_page_device_data->uptime->mutex);

    result = httpd_resp_send_chunk(req, str, HTTPD_RESP_USE_STRLEN);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send a chunk of the response");
        return httpd_resp_send_500(req);
    }

    char sensor_text[30] = {0};

    (void) xSemaphoreTake(device_info_page_device_data->user_settings->mutex, portMAX_DELAY);
    if (device_info_page_device_data->user_settings->i2c_devices_daq_temp_sensor_select == 0) {
        strcpy(sensor_text, "AM2320");
    } else if (device_info_page_device_data->user_settings->i2c_devices_daq_temp_sensor_select == 1) {
        strcpy(sensor_text, "BMP280");
    } else {
        strcpy(sensor_text, "AM2320s and BMP280s mean");
    }
	char tmp_tz_code[10] = {0};
	strcpy(tmp_tz_code, device_info_page_device_data->user_settings->tz_code);
	if (tmp_tz_code[3] == '+') {
		tmp_tz_code[3] = '-';
	} else if (tmp_tz_code[3] == '-') {
		tmp_tz_code[3] = '+';
	}
    sprintf(
        str,
        "document.getElementById('timezone').textContent = '%s';"
        "document.getElementById('mdns_hostname').textContent = '%s';"
        "document.getElementById('temp_sensor').textContent = '%s';"
        "</script></body></html>",
        tmp_tz_code,
        device_info_page_device_data->user_settings->mdns_hostname,
        sensor_text
    );
    (void) xSemaphoreGive(device_info_page_device_data->user_settings->mutex);

    result = httpd_resp_send_chunk(req, str, HTTPD_RESP_USE_STRLEN);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send a chunk of the response");
        return httpd_resp_send_500(req);
    }

    result = httpd_resp_send_chunk(req, NULL, 0);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "Failed to send the final part of the page of length 0");
		return httpd_resp_send_500(req);
	}

    return ESP_OK;
}

httpd_uri_t device_info_page = {
    .uri     = "/device_info",
    .method  = HTTP_GET,
    .handler = device_info_handler,
};
