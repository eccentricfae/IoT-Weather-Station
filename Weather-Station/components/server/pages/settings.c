#include "server.h"

char const settings_data[] = 
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
	"            margin: 20px 0;\n"
	"        }\n"
	"\n"
	"        .hide_at_start {\n"
	"            display: none;\n"
	"        }\n"
	"\n"
	"        .accordion_button {\n"
	"            width: 100%;\n"
	"            color: #7d34b2;\n"
	"            background-color: white;\n"
	"            background-image: linear-gradient(to right, #b57edc, white, #b57edc);\n"
	"            border: 3px solid #7d34b2;\n"
	"            border-radius: 10px;\n"
	"            margin-top: 20px;\n"
	"            font-size: large;\n"
	"            font-weight: bold;\n"
	"            padding-top: 10px;\n"
	"            padding-bottom: 10px;\n"
	"        }\n"
	"\n"
	"        .accordion_button:hover {\n"
	"            background-image: linear-gradient(to right, #9967bd, rgb(217, 217, 217), #9967bd);\n"
	"        }\n"
	"\n"
	"        .accordion_container {\n"
	"            width: 97%;\n"
	"            background-color: white;\n"
	"            border-top: 1px solid #dcbaf5;\n"
	"            border-right: 3px solid #7d34b2;\n"
	"            border-left: 3px solid #7d34b2;\n"
	"            border-bottom: 3px solid #7d34b2;\n"
	"            border-bottom-left-radius: 10px;\n"
	"            border-bottom-right-radius: 10px;\n"
	"            margin: auto;\n"
	"        }\n"
	"\n"
	"\n"
	"\n"
	"        .setting_element {\n"
	"            /* Placeholder class - DO NOT DELETE! */\n"
	"        }\n"
	"\n"
	"        .setting_element_div {\n"
	"            display: flex;\n"
	"            flex-direction: column;\n"
	"            margin-top: 3px;\n"
	"            margin-bottom: 3px;\n"
	"            margin-left: 10px;\n"
	"            margin-right: 10px;\n"
	"        }\n"
	"\n"
	"        .setting_element_div > hr {\n"
	"            width: 97%;\n"
	"            border: 1px solid #b57edc;\n"
	"        }\n"
	"\n"
	"        .setting_element_div > p {\n"
	"            margin: 0;\n"
	"            margin-top: 5px;\n"
	"            padding: 0;\n"
	"            font-size: small;\n"
	"        }\n"
	"\n"
	"        .setting_element_div > .sed2 {\n"
	"            display: flex;\n"
	"            flex-direction: row;\n"
	"        }\n"
	"\n"
	"        .setting_element_div > .sed2 > *{\n"
	"            width: 50%;\n"
	"        }\n"
	"\n"
	"        .setting_element_div > .sed2 > input, select {\n"
	"            border: 1px solid #b57edc;\n"
	"            border-radius: 10px;\n"
	"            background-color: white;\n"
	"            text-align: center;\n"
	"        }\n"
	"\n"
	"        .setting_element_div > input:focus {\n"
	"            outline: none !important;\n"
	"        }\n"
	"\n"
	"        #app_title {\n"
	"            text-align: center;\n"
	"            color: #7d34b2;\n"
	"            text-shadow: 0px 0px 5px black;\n"
	"            margin: auto;\n"
	"            border: 3px solid #7d34b2;\n"
	"            border-radius: 20px;\n"
	"            padding: 10px;\n"
	"            width: 75%;\n"
	"            background-color: white;\n"
	"        }\n"
	"\n"
	"        #list_of_accordions {\n"
	"            text-align: center;\n"
	"        }\n"
	"\n"
	"        #save_button {\n"
	"            margin-top: 20px;\n"
	"            border: 3px solid #7d34b2;\n"
	"            color: #7d34b2;\n"
	"            border-radius: 10px;\n"
	"            font-size: large;\n"
	"            font-weight: bold;\n"
	"            padding: 10px;\n"
	"            background-image: linear-gradient(to right, #b57edc, 10%, white, 90%, #b57edc);\n"
	"        }\n"
	"\n"
	"        #save_button:hover {\n"
	"            background-image: linear-gradient(to right, #9967bd, 10%, rgb(217, 217, 217), 90%, #9967bd);\n"
	"        }\n"
	"\n"
	"        #instructions > p {\n"
	"            margin-left: 10px;\n"
	"            margin-right: 10px;\n"
	"        }\n"
	"        #error_alert {\n"
	"            display: none;\n"
	"            border: 3px solid red;\n"
	"            border-radius: 20px;\n"
	"            background-color: lightcoral;\n"
	"            margin: auto;\n"
	"            margin-top: 20px;\n"
	"            width: 350px;\n"
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
	"            <h1>Weather Station's Settings</h1>\n"
	"        </div>\n"
	"\n"
	"        <div id='list_of_accordions'>\n"
	"            <button type='button' id='basic_settings_button' onclick='accordion_toggle1()' class='accordion_button'>BASIC SETTINGS ▼</button>\n"
	"            <div id='basic_settings' class='hide_at_start accordion_container'>\n"
	"                <div class='setting_element_div'>\n"
	"                    <div class='sed2'>\n"
	"                        <p>WiFi SSID</p>\n"
	"                        <input type='text' id='wifi_name' class='setting_element'>\n"
	"                    </div>\n"
	"                    <p class='tooltip'>The name of the WiFi network that the device should connect to</p>\n"
	"                    <hr>\n"
	"                </div>\n"
	"                <div class='setting_element_div'>\n"
	"                    <div class='sed2'>\n"
	"                        <p>WiFi Password</p>\n"
	"                        <input type='text' id='wifi_password' class='setting_element'>\n"
	"                    </div>\n"
	"                    <p class='tooltip'>The password to the WiFi network that the device should connect to</p>\n"
	"                    <hr>\n"
	"                </div>\n"
	"                <div class='setting_element_div'>\n"
	"                    <div class='sed2'>\n"
	"                        <p>TimeZone</p>\n"
	"                        <select id='tz_code' name='tz_code' class='setting_element'>\n"
	"                            <option value='GMT+12'>GMT -12:00</option>\n"
	"                            <option value='GMT+11'>GMT -11:00</option>\n"
	"                            <option value='GMT+10'>GMT -10:00</option>\n"
	"                            <option value='GMT+09'>GMT -9:00</option>\n"
	"                            <option value='GMT+08'>GMT -8:00</option>\n"
	"                            <option value='GMT+07'>GMT -7:00</option>\n"
	"                            <option value='GMT+06'>GMT -6:00</option>\n"
	"                            <option value='GMT+05'>GMT -5:00</option>\n"
	"                            <option value='GMT+04'>GMT -4:00</option>\n"
	"                            <option value='GMT+03'>GMT -3:00</option>\n"
	"                            <option value='GMT+02'>GMT -2:00</option>\n"
	"                            <option value='GMT+01'>GMT -1:00</option>\n"
	"                            <option value='GMT' selected='selected'>GMT</option>\n"
	"                            <option value='GMT-01'>GMT +1:00</option>\n"
	"                            <option value='GMT-02'>GMT +2:00</option>\n"
	"                            <option value='GMT-03'>GMT +3:00</option>\n"
	"                            <option value='GMT-04'>GMT +4:00</option>\n"
	"                            <option value='GMT-05'>GMT +5:00</option>\n"
	"                            <option value='GMT-06'>GMT +6:00</option>\n"
	"                            <option value='GMT-07'>GMT +7:00</option>\n"
	"                            <option value='GMT-08'>GMT +8:00</option>\n"
	"                            <option value='GMT-09'>GMT +9:00</option>\n"
	"                            <option value='GMT-10'>GMT +10:00</option>\n"
	"                            <option value='GMT-11'>GMT +11:00</option>\n"
	"                            <option value='GMT-12'>GMT +12:00</option>\n"
	"                            <option value='GMT-13'>GMT +13:00</option>\n"
	"                            <option value='GMT-14'>GMT +14:00</option>\n"
	"                        </select>\n"
	"                    </div>\n"
	"                    <p class='tooltip'>The timezone in which the device will be working - used for automatic date and time synchronization</p>\n"
	"                    <hr>\n"
	"                </div>\n"
	"\n"
	"            </div>\n"
	"\n"
	"            <button type='button' id='advanced_settings_button' onclick='accordion_toggle2()' class='accordion_button'>ADVANCED SETTINGS ▼</button>\n"
	"            <div id='advanced_settings' class='hide_at_start accordion_container'>\n"
	"                <div class='setting_element_div'>\n"
	"                    <div class='sed2'>\n"
	"                        <p>mDNS Hostname</p>\n"
	"                        <input type='text' value='weatherstation' id='mdns_hostname' class='setting_element'>\n"
	"                    </div>\n"
	"                    <p class='tooltip'>The name that the device should be known as on the local network. If set to e.g. 'esp' the device can be accessed as 'esp.local' in the browser</p>\n"
	"                    <hr>\n"
	"                </div>\n"
	"                <div class='setting_element_div'>\n"
	"                    <div class='sed2'>\n"
	"                        <p>Serial Logging Verbosity</p>\n"
	"                        <select id='serial_logging_verbosity' name='serial_logging_verbosity' class='setting_element'>\n"
	"                            <option value='0'>None</option>\n"
	"                            <option value='1'>Error</option>\n"
	"                            <option value='2'>Warning</option>\n"
	"                            <option value='3'>Info</option>\n"
	"                            <option value='4' selected='selected'>Debug</option>\n"
	"                            <option value='5'>Verbose</option>\n"
	"                        </select>\n"
	"                    </div>\n"
	"                    <p class='tooltip'>The verbosity of the logging the device will do on the serial connection to the PC</p>\n"
	"                    <hr>\n"
	"                </div>\n"
	"                <div class='setting_element_div'>\n"
	"                    <div class='sed2'>\n"
	"                        <p>WiFi RSSI test period</p>\n"
	"                        <input type='number' value='60000' min='10' max='3600000' id='wifi_rssi_test_period_ms' class='setting_element'>\n"
	"                    </div>\n"
	"                    <p class='tooltip'>The period (in milliseconds) of testing the strength of the device's WiFi connection</p>\n"
	"                    <hr>\n"
	"                </div>\n"
	"                <div class='setting_element_div'>\n"
	"                    <div class='sed2'>\n"
	"                        <p>Data acquisition period</p>\n"
	"                        <input type='number' value='1000' min='5' max='3600000' id='i2c_devices_daq_interval_ms' class='setting_element'>\n"
	"                    </div>\n"
	"                    <p class='tooltip'>The period (in milliseconds) of getting the data from the device's sensors</p>\n"
	"                    <hr>\n"
	"                </div>\n"
	"                <div class='setting_element_div'>\n"
	"                    <div class='sed2'>\n"
	"                        <p>Device's temperature sensor select</p>\n"
	"                        <select id='i2c_devices_daq_temp_sensor_select' name='i2c_devices_daq_temp_sensor_select' class='setting_element'>\n"
	"                            <option value='0'>AM2320</option>\n"
	"                            <option value='1'>BMP280</option>\n"
	"                            <option value='2' selected='selected'>Average of the two</option>\n"
	"                        </select>\n"
	"                    </div>\n"
	"                    <p class='tooltip'>Which sensor should the device use for temperature measurment</p>\n"
	"                    <hr>\n"
	"                </div>\n"
	"                <div class='setting_element_div'>\n"
	"                    <div class='sed2'>\n"
	"                        <p>Data Write Interval</p>\n"
	"                        <input type='number' value='60' min='1' max='3600000' id='data_write_to_sdcard_interval' class='setting_element'>\n"
	"                    </div>\n"
	"                    <p class='tooltip'>Interval in seconds between each write of data to the file</p>\n"
	"                    <hr>\n"
	"                </div>\n"
	"            </div>\n"
	"\n"
	"            <div id='error_alert'>\n"
	"                <h3>Failed to update the settings!</h3>\n"
	"            </div>\n"
	"\n"
	"            <button id='save_button' onclick='save_settings()'>Save Settings</button>\n"
	"        </div>\n"
	"    </section>\n"
	"\n"
	"    <script>\n"
	"        var accordion_containers = document.getElementsByClassName('accordion_container');\n"
	"        for (var i = 0; i < accordion_containers.length; i++) {\n"
	"            accordion_containers[i].style.display = 'none';\n"
	"        }\n"
	"\n"
	"        function accordion_toggle1() {\n"
	"            accordion_toggle('basic_settings');\n"
	"        }\n"
	"\n"
	"        function accordion_toggle2() {\n"
	"            accordion_toggle('advanced_settings');\n"
	"        }\n"
	"\n"
	"\n"
	"        function accordion_toggle(id) {\n"
	"            var button = document.getElementById(id + '_button');\n"
	"            var div = document.getElementById(id);\n"
	"            if (!div) {\n"
	"                return;\n"
	"            }\n"
	"\n"
	"            if (div.style.display === 'none') {\n"
	"                div.style.display = 'block';\n"
	"                button.innerHTML = button.innerHTML.slice(0, -1) + '▲';\n"
	"            } else {\n"
	"                div.style.display = 'none';\n"
	"                button.innerHTML = button.innerHTML.slice(0, -1) + '▼';\n"
	"            }\n"
	"        }\n"
	"\n"
	"        function save_settings() {\n"
	"            var setting_elements = document.getElementsByClassName('setting_element');\n"
	"\n"
	"            var settings_json = {};\n"
	"\n"
	"            for (var i = 0; i < setting_elements.length; i++) {\n"
	"                var name = setting_elements[i].id;\n"
	"                var value = setting_elements[i].value;\n"
	"\n"
	"                if (isNaN(value)) {\n"
	"                    settings_json[name] = value;\n"
	"                } else {\n"
	"                    settings_json[name] = Number(value);\n"
	"                }\n"
	"            }\n"
	"\n"
	"            let restart = confirm('Setting new settings requires the device to be restarted. Do You wish to proceed?');\n"
	"\n"
	"            fetch(\n"
	"                window.location.href,\n"
	"                {\n"
	"                    method: 'POST',\n"
	"                    body: JSON.stringify(settings_json),\n"
	"                    headers: {\n"
	"                        'Restart-Requested': restart.toString()\n"
	"                    },\n"
	"                }\n"
	"            )\n"
	"            .then((response) => {\n"
	"                if (response.status === 200) {\n"
	"                    location.reload();\n"
	"                } else {\n"
	"                    document.getElementById('error_alert').style.display = 'block';\n"
	"                }\n"
	"            });\n"
	"        }\n"
	"    </script>\n"
	"    <script>\n"
	"\n"
;

// !@#$%

#include "sd_card.h"
#include "i2c_devices.h"

#define TAG "settings.c"

data_aggregate_t const * settings_page_device_data = NULL;

esp_err_t settings_handler(httpd_req_t * req)
{
	esp_err_t result = ESP_OK;

	if (settings_page_device_data == NULL) {
		ESP_LOGE(TAG, "settings_page_device_data was not set!");
		return httpd_resp_send_500(req);
	}

    result = httpd_resp_send_chunk(req, settings_data, HTTPD_RESP_USE_STRLEN);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "Failed sending the page's main content (settings_data) chunk");
		return httpd_resp_send_500(req);
	}

    char str[768] = {0};

    (void) xSemaphoreTake(settings_page_device_data->user_settings->mutex, portMAX_DELAY);
    sprintf(
        str,
		"document.getElementById('wifi_name').value = '%s';"
		"document.getElementById('wifi_password').value = '%s';"
		"document.getElementById('tz_code').value = '%s';"
		"document.getElementById('mdns_hostname').value = '%s';"
		"document.getElementById('serial_logging_verbosity').value = '%d';"
		"document.getElementById('wifi_rssi_test_period_ms').value = '%ld';"
		"document.getElementById('i2c_devices_daq_interval_ms').value = '%ld';"
		"document.getElementById('i2c_devices_daq_temp_sensor_select').value = '%ld';"
		"document.getElementById('data_write_to_sdcard_interval').value = '%ld';"
		"</script> </body> </html>",
        settings_page_device_data->user_settings->wifi_name,
        settings_page_device_data->user_settings->wifi_password,
        settings_page_device_data->user_settings->tz_code,
        settings_page_device_data->user_settings->mdns_hostname,
        settings_page_device_data->user_settings->serial_logging_verbosity,
        settings_page_device_data->user_settings->wifi_rssi_test_period_ms,
        settings_page_device_data->user_settings->i2c_devices_daq_interval_ms,
        settings_page_device_data->user_settings->i2c_devices_daq_temp_sensor_select,
        settings_page_device_data->user_settings->data_write_to_sdcard_interval
    );
    (void) xSemaphoreGive(settings_page_device_data->user_settings->mutex);

    result = httpd_resp_send_chunk(req, str, HTTPD_RESP_USE_STRLEN);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "Failed sending the page's dynamic content (settings_data) chunk");
		return httpd_resp_send_500(req);
	}

	result = httpd_resp_send_chunk(req, NULL, 0);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "Failed to send the final part of the page of length 0");
		return httpd_resp_send_500(req);
	}

    return ESP_OK;
}

httpd_uri_t settings_page = {
    .uri     = "/settings",
    .method  = HTTP_GET,
    .handler = settings_handler,
};

esp_err_t settings_post_handler(httpd_req_t * req)
{
	esp_err_t result = ESP_OK;

	if (settings_page_device_data == NULL) {
		ESP_LOGE(TAG, "settings_page_device_data was not set!");
		return httpd_resp_send_500(req);
	}

    char json[300] = {0};
	// The settings.txt length is around 250ish characters
	// if the received data from request's body is longer than that somewthing went wrong

	int bytes = httpd_req_recv(req, json, sizeof(json));
	if (bytes == 0) {
		ESP_LOGE(TAG, "POST HANDLER: Connection closed by the peer before we got the body data");
		return httpd_resp_send_500(req);
	} else if (bytes < 0) {
		ESP_LOGE(TAG, "POST HANDLER: Error while getting the request's body's data");
		return httpd_resp_send_500(req);
	}

    result = sd_card_write("settings.txt", json, OVERWRITE);
    if (result != ESP_OK) {
		ESP_LOGE(TAG, "Failed to save the settings on the SD card");
		return httpd_resp_send_500(req);
    }

	result = httpd_resp_send_chunk(req, NULL, 0);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "POST HANDLER: Failed to respond with code 200");
		return httpd_resp_send_500(req);
	}

    LCD_clear();
    esp_restart();

    return ESP_OK;
}

httpd_uri_t settings_post_page = {
    .uri     = "/settings",
    .method  = HTTP_POST,
    .handler = settings_post_handler,
};
