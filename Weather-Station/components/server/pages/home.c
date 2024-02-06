#include "server.h"

char const home_data[] = 
	"<!DOCTYPE html>\n"
	"<html lang='en'>\n"
	"<head>\n"
	"    <meta charset='UTF-8'>\n"
	"    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
	"    <title>Weather Station | Home Page</title>\n"
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
	"        .weather-info {\n"
	"            display: flex;\n"
	"            justify-content: space-around;\n"
	"            align-items: center;\n"
	"            flex-wrap: wrap;\n"
	"        }\n"
	"\n"
	"        .weather-card {\n"
	"            width: 200px;\n"
	"            padding: 10px;\n"
	"            margin: 10px;\n"
	"            border: 1px solid #ddd;\n"
	"            border-radius: 8px;\n"
	"            box-shadow: 0 0 10px #7d34b2;\n"
	"        }\n"
	"\n"
	"        .weather-card > h2, p {\n"
	"            text-align: center;\n"
	"        }\n"
	"\n"
	"        .chart_nav {\n"
	"            margin: auto;\n"
	"            margin-top: 50px;\n"
	"            background-color: #7d34b2;\n"
	"            border: 2px solid #7d34b2;\n"
	"            border-radius: 20px;\n"
	"            padding-bottom: 10px;\n"
	"            text-align: center;\n"
	"        }\n"
	"\n"
	"        @media screen and (min-width: 480px) {\n"
	"            .chart_nav > div {\n"
	"                width: 450px;\n"
	"                display: flex;\n"
	"                flex-direction: row;\n"
	"                margin: auto;\n"
	"            }\n"
	"            .chart_nav_btn {\n"
	"                width: 150px;\n"
	"            }\n"
	"        }\n"
	"\n"
	"        @media screen and (max-width: 480px) {\n"
	"            .chart_nav > div {\n"
	"                display: flex;\n"
	"                flex-direction: column;\n"
	"            }\n"
	"            .chart_nav_btn {\n"
	"                width: 93%;\n"
	"            }\n"
	"        }\n"
	"\n"
	"        .chart_nav > h2 {\n"
	"            color: white;\n"
	"        }\n"
	"\n"
	"        .chart_nav_btn {\n"
	"            background-color: white;\n"
	"            color: #7d34b2;\n"
	"            border: 1px solid #7d34b2;\n"
	"            border-radius: 10px;\n"
	"            margin-right: 10px;\n"
	"            margin-left: 10px;\n"
	"            padding: 10px;\n"
	"        }\n"
	"\n"
	"        .chart_nav_btn:hover {\n"
	"            box-shadow: 0px 0px 20px yellow;\n"
	"        }\n"
	"\n"
	"        #chart_canvas {\n"
	"            margin-top: 50px;\n"
	"        }\n"
	"\n"
	"        #list_of_files > li {\n"
	"            background-color: white;\n"
	"            margin-top: 30px;\n"
	"            margin-left: calc(50% - 90px);\n"
	"        }\n"
	"\n"
	"        #list_of_files > li > a {\n"
	"            color: #7d34b2;\n"
	"            background-color: white;\n"
	"            border: 1px solid #7d34b2;\n"
	"            border-radius: 20px;\n"
	"            padding: 10px;\n"
	"            text-decoration:none;\n"
	"            margin-left: 5px;\n"
	"            margin-right: 5px;\n"
	"        }\n"
	"\n"
	"        #list_of_files > li > a:hover {\n"
	"            background-color: gray;\n"
	"        }\n"
	"\n"
	"        ul {\n"
	"            list-style-type: none;\n"
	"        }\n"
	"    </style>\n"
	"</head>\n"
	"<body>\n"
	"    <header>\n"
	"        <div class='header-buttons'>\n"
	"            <a class='header-button' href='./'>Home</a>\n"
	"            <a class='header-button' href='./device_info'>Device info</a>\n"
	"            <a class='header-button' href='./settings'>Settings</a>\n"
	"        </div>\n"
	"    </header>\n"
	"\n"
	"    <section>\n"
	"        <div id='body_title'>\n"
	"            <h1>Weather Station's Main Page</h1>\n"
	"        </div>\n"
	"\n"
	"        <div class='weather-info'>\n"
	"            <div class='weather-card'>\n"
	"                <h2>Temperature</h2>\n"
	"                <h2><span id='temp'>25°C</span></h2>\n"
	"            </div>\n"
	"            <div class='weather-card'>\n"
	"                <h2>Humidity</h2>\n"
	"                <h2><span id='humd'>34%</span></h2>\n"
	"            </div>\n"
	"            <div class='weather-card'>\n"
	"                <h2>Pressure</h2>\n"
	"                <h2><span id='pres'>940.32 Pa</span></h2>\n"
	"            </div>\n"
	"        </div>\n"
	"\n"
	"        <div class='chart_nav'>\n"
	"            <h2 id='chart_main_title'>Today's Charted Data</h2>\n"
	"            <div>\n"
	"                <button class='chart_nav_btn' onclick='show_temperature()'>Temperature</button>\n"
	"                <button class='chart_nav_btn' onclick='show_humidity()'>Humidity</button>\n"
	"                <button class='chart_nav_btn' onclick='show_pressure()'>Pressure</button>\n"
	"            </div>\n"
	"        </div>\n"
	"\n"
	"        <canvas id='chart_canvas'></canvas>\n"
	"\n"
	"        <div class='chart_nav'>\n"
	"            <h2>File Download</h2>\n"
	"            <h2>Click the name of the file you wish to download!</h2>\n"
	"        </div>\n"
	"\n"
	"        <ul id='list_of_files'></ul>\n"
	"\n"
	"    </section>\n"
	"    <script src='https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js'></script>\n"
	"    <script>\n"
	"    function new_chart(x, y, x_name, y_name, min, max) {\n"
	"        let chart = new Chart('chart_canvas', {\n"
	"            type: 'line',\n"
	"            data: {\n"
	"                labels: x,\n"
	"                datasets: [{\n"
	"                data: y,\n"
	"                borderColor: 'blue',\n"
	"                fill: false\n"
	"                }]\n"
	"            },\n"
	"            options: {\n"
	"                legend: {display: false},\n"
	"                scales: {\n"
	"                    xAxes: [{\n"
	"                        stacked: true,\n"
	"                        scaleLabel: {\n"
	"                            display: true,\n"
	"                            labelString: x_name\n"
	"                        }\n"
	"                    }],\n"
	"                    yAxes: [{\n"
	"                        ticks: {\n"
	"                            beginAtZero: false,\n"
	"                            suggestedMax: max,\n"
	"                            suggestedMin: min\n"
	"                        },\n"
	"                        scaleLabel: {\n"
	"                            display: true,\n"
	"                            labelString: y_name\n"
	"                        }\n"
	"                    }]\n"
	"                }\n"
	"            }\n"
	"        });\n"
	"    }\n"
	"\n"
	"    function show_temperature() {\n"
	"        new_chart(chart_data[0], chart_data[1], 'Timepoint', 'Temperature [C]', Math.min(...chart_data[1]) - 3, Math.max(...chart_data[1]) + 3);\n"
	"    }\n"
	"\n"
	"    function show_humidity() {\n"
	"        new_chart(chart_data[0], chart_data[2], 'Timepoint', 'Humidity [%]', 0, 100);\n"
	"    }\n"
	"\n"
	"    function show_pressure() {\n"
	"        new_chart(chart_data[0], chart_data[3], 'Timepoint', 'Pressure [Pa]', 90000, 110000);\n"
	"    }\n"
;

// !@#$%

#define TAG "home.c"

data_aggregate_t const * home_page_device_data = NULL;

esp_err_t send_file_handler(httpd_req_t * req, char * filename)
{
	esp_err_t result = ESP_OK;

    bool file_exists = sd_card_file_exists(filename) == ESP_OK;

    if (file_exists == false) {
        ESP_LOGE(TAG, "The file requested by the user was not found!");
        return httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "The requested file was not found!");
    }

    (void) httpd_resp_set_type(req, "text/csv");

	char hdr_tmp[64] = {0};
	sprintf(hdr_tmp, "attachment; filename=\"%s\"", filename);
	(void) httpd_resp_set_hdr(req, "Content-Disposition", hdr_tmp);

    FILE * file = sd_card_fopen(filename, "r");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open the %s file", filename);
        return httpd_resp_send_500(req);
    }

    char buffer[256] = {0};

    while (fgets(buffer, sizeof(buffer), file)) {
        result = httpd_resp_sendstr_chunk(req, buffer);
        if (result != ESP_OK) {
            ESP_LOGE(TAG, "Failed to send chunk of data to the client");
            return httpd_resp_send_500(req);
        }

        memset(buffer, 0, sizeof(buffer));
    }

    fclose(file);

	result = httpd_resp_send_chunk(req, NULL, 0);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send last chunk of data to the client");
        return httpd_resp_send_500(req);
    }

	return ESP_OK;
}

esp_err_t home_handler(httpd_req_t * req)
{
	esp_err_t result = ESP_OK;

	if (home_page_device_data == NULL) {
		ESP_LOGE(TAG, "home_page_device_data was not set!");
		return httpd_resp_send_500(req);
	}

	char filename[14] = {0};
	bool set_new_chart_title = false;

	size_t url_len = httpd_req_get_url_query_len(req);
    if (url_len == 0) {
		// send_file = false;
    } else if (url_len >= 35) {
        ESP_LOGE(TAG, "The URL query is longer than 35 characters. Should never be the case!");
        return httpd_resp_send_500(req);
    }
	
	if (url_len > 0) {
		char url[50] = {0};
		result = httpd_req_get_url_query_str(req, url, sizeof(url));
		if (result != ESP_OK) {
			ESP_LOGE(TAG, "Failed to get the URL query string");
			return httpd_resp_send_500(req);
		}

		result = httpd_query_key_value(url, "download", filename, sizeof(filename));
		if (result != ESP_OK) {
			if (result == ESP_ERR_NOT_FOUND) {
				// ESP_LOGE(TAG, "The filename key-value pair was not found in the URL query");
				// return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "The URL query requires key-value argument pair of key \"filename\" to select what file to download");
			} else if (result == ESP_ERR_HTTPD_RESULT_TRUNC) {
				ESP_LOGE(TAG, "The filename value is not valid! Longer than 13 characters");
				return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "The filename value is too long. The filenames are always in format of ddmmyyyy.txt and are always of length 12");
			} else {
				ESP_LOGE(TAG, "Other error while getting key-value pair for filename");
				return httpd_resp_send_500(req);
			}
		} else {
			return send_file_handler(req, filename);
		}

		result = httpd_query_key_value(url, "chart_file", filename, sizeof(filename));
		if (result != ESP_OK) {
			if (result == ESP_ERR_NOT_FOUND) {
				//
			} else if (result == ESP_ERR_HTTPD_RESULT_TRUNC) {
				ESP_LOGE(TAG, "The filename value is not valid! Longer than 13 characters");
				return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "The filename value is too long. The filenames are always in format of ddmmyyyy.txt and are always of length 12");
			} else {
				ESP_LOGE(TAG, "Other error while getting key-value pair for filename");
				return httpd_resp_send_500(req);
			}
		} else {
			set_new_chart_title = true;
		}
	}

	result = httpd_resp_send_chunk(req, home_data, HTTPD_RESP_USE_STRLEN);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "Failed sending the page's main content (home_data) chunk");
		return httpd_resp_send_500(req);
	}

	char str[256] = {0};

	if (set_new_chart_title) {
		// Change the chart's main title from "Today's" to the name of the file
		sprintf(str, "document.getElementById('chart_main_title').textContent = '%s Charted Data';\n", filename);
		result = httpd_resp_sendstr_chunk(req, str);
		if (result != ESP_OK) {
			ESP_LOGE(TAG, "Failed to send the dynamic (data) part of the page");
			return httpd_resp_send_500(req);
		}
	}

    (void) xSemaphoreTake(home_page_device_data->i2c_daq_data->mutex, portMAX_DELAY);
	sprintf(
		str, 
		"document.getElementById('temp').textContent = '%.1f °C';\n"
		"document.getElementById('humd').textContent = '%.1f %%';\n"
		"document.getElementById('pres').textContent = '%d hPa';\n"
		,
		home_page_device_data->i2c_daq_data->temperature_C,
		home_page_device_data->i2c_daq_data->humidity_P,
		(int)(home_page_device_data->i2c_daq_data->pressure_Pa) / 100
	);
    (void) xSemaphoreGive(home_page_device_data->i2c_daq_data->mutex);

	result = httpd_resp_send_chunk(req, str, HTTPD_RESP_USE_STRLEN);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "Failed to send the dynamic (data) part of the page");
		return httpd_resp_send_500(req);
	}

	result = httpd_resp_send_chunk(
		req, 
		"let chart_data = [ [], [], [], [] ];\n", 
		HTTPD_RESP_USE_STRLEN
	);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "Failed to send the dynamic (data) part of the page");
		return httpd_resp_send_500(req);
	}

	if (url_len <= 0) {
		// File for charting -> todays date
		memset(filename, 0, sizeof(filename));
		strftime(filename, 13, "%d%m%Y.txt", &(home_page_device_data->uptime->date_time));
	} else {
		// Do nothing
		// Received filename for charting as URL's GET argument
		// and already should be in the "filename" char[]
	}

	FILE * file = sd_card_fopen(filename, "r");
	if (file == NULL) {
		ESP_LOGE(TAG, "Failed to open the %s file", filename);
        goto failed_chart;
	}
	
	if (home_page_device_data->uptime->date_time.tm_year == 70
	    || home_page_device_data->uptime->date_time.tm_year == 69) {
		// Not sync'ed datetime yet
		goto failed_chart;
	}

    char buffer[256] = {0};
	// 	"chart_data[0].push(\"12345678\");"
	// 	"chart_data[1].push(12.4);"
	// 	"chart_data[2].push(12.4);"
	// 	"chart_data[3].push(12345.7);\n"
	char send_buff[128] = {0};
	char time_buff[10]  = {0};
	char temp_buff[10]  = {0};
	char humd_buff[10]  = {0};
	char pres_buff[10]  = {0};
	bool first_iter = true;
	int comma_idx = 0;
	int last_comma_idx = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
		if (first_iter) {
			first_iter = false;
			continue;
		}

		strncpy(time_buff, buffer, 8);
		comma_idx = 8;
		last_comma_idx = 8;
		sprintf(send_buff, "chart_data[0].push(\"%s\");", time_buff);

		result = httpd_resp_sendstr_chunk(req, send_buff);
        if (result != ESP_OK) {
            ESP_LOGE(TAG, "Failed to send chunk of data to the client");
            goto failed_chart;
        }

		comma_idx++;
		while (buffer[comma_idx] != ',') {
			comma_idx++;
		}
		strncpy(temp_buff, buffer + last_comma_idx + 1,  comma_idx - last_comma_idx - 1);
		sprintf(send_buff, "chart_data[1].push(%s);", temp_buff);
		last_comma_idx = comma_idx;

		result = httpd_resp_sendstr_chunk(req, send_buff);
        if (result != ESP_OK) {
            ESP_LOGE(TAG, "Failed to send chunk of data to the client");
            goto failed_chart;
        }

		comma_idx++;
		while (buffer[comma_idx] != ',') {
			comma_idx++;
		}
		strncpy(humd_buff, buffer + last_comma_idx + 1, comma_idx - last_comma_idx - 1);
		sprintf(send_buff, "chart_data[2].push(%s);", humd_buff);
		last_comma_idx = comma_idx;
		
		result = httpd_resp_sendstr_chunk(req, send_buff);
        if (result != ESP_OK) {
            ESP_LOGE(TAG, "Failed to send chunk of data to the client");
            goto failed_chart;
        }
		
		while (buffer[comma_idx] != '.') {
			comma_idx++;
		}
		comma_idx += 2;
		strncpy(pres_buff, buffer + last_comma_idx + 1, comma_idx - last_comma_idx - 1);
		sprintf(send_buff, "chart_data[3].push(%s);\n", pres_buff);

		result = httpd_resp_sendstr_chunk(req, send_buff);
        if (result != ESP_OK) {
            ESP_LOGE(TAG, "Failed to send chunk of data to the client");
            goto failed_chart;
        }
    }

	result = httpd_resp_send_chunk(req, "show_temperature();\n", HTTPD_RESP_USE_STRLEN);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "Failed to send the chart-data part of the page");
		goto failed_chart;
	}
	
	failed_chart:
    if (file != NULL) {
		fclose(file);
		file = NULL;
	}

    DIR * dir = opendir(SD_CARD_MOUNTPOINT);
    if (dir == NULL) {
        ESP_LOGE(TAG, "Failed to open the sd card's main directory");
		return httpd_resp_send_500(req);
    }

    struct dirent * dir_elem = NULL;

    sprintf(
        str, 
        "var files_list = document.getElementById('list_of_files');\n\n"
    );
    result = httpd_resp_sendstr_chunk(req, str);
    if (result != ESP_OK) {
		ESP_LOGE(TAG, "Failed to send the dynamic (data) part of the page");
		return httpd_resp_send_500(req);
    }

	char f_tmp[15] = {0};

	while ((dir_elem = readdir(dir))) {
		if (dir_elem->d_type != DT_REG) {
			continue;
		}

		int filename_len = strlen(dir_elem->d_name);
		if (filename_len > 13) {
			// Not an application's file's name -> user's file -> skip
			continue;
		}
		
		(void) strcpy(f_tmp, dir_elem->d_name);

		bool is_settings_file = strcmp(f_tmp, "SETTINGS.TXT") == 0;
		
		bool is_data_file = true;
		for (int i = 0; i < filename_len; i++) {
			if (i < 8) {
				if (f_tmp[i] < '0' || f_tmp[i] > '9') {
					// Char in the name is not a number but should be!
					is_data_file = false;
					break;
				}
			} else {
				bool is_txt = strncmp(f_tmp + i, ".TXT", 4) == 0;
				is_data_file = is_txt;
				break;
			}
		}

        sprintf(
            str, 
            "var li_new_elem = document.createElement('li');\n"
            "var link = document.createElement('a');\n"
            "link.href = './?download=%s';\n"
            "link.textContent = '%s';\n"
            "li_new_elem.appendChild(link)\n"
			"%s"
            ,
            f_tmp,
            f_tmp,
			// If is settings file -> append it to the list, otherwise the section that sends "Display" button will append both of the buttons to the list
			is_settings_file ? "files_list.appendChild(li_new_elem);\n\n" : ""
        );

        result = httpd_resp_sendstr_chunk(req, str);
        if (result != ESP_OK) {
			ESP_LOGE(TAG, "Failed to send the dynamic (data) part of the page");
			return httpd_resp_send_500(req);
        }

		if (is_settings_file == false) {
			sprintf(
				str, 
				"var link2 = document.createElement('a');\n"
				"link2.href = './?chart_file=%s';\n"
				"link2.textContent = 'Display';\n"
				"li_new_elem.appendChild(link2)\n"
				"files_list.appendChild(li_new_elem);\n\n"
				,
				f_tmp
			);

			result = httpd_resp_sendstr_chunk(req, str);
			if (result != ESP_OK) {
				ESP_LOGE(TAG, "Failed to send the dynamic (data) part of the page");
				return httpd_resp_send_500(req);
			}
		}

        memset(str, 0, sizeof(str));
    }

	result = httpd_resp_send_chunk(req, "</script>\n</body>\n</html>\n", HTTPD_RESP_USE_STRLEN);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "Failed to send the dynamic (data) part of the page");
		return httpd_resp_send_500(req);
	}

	result = httpd_resp_send_chunk(req, NULL, 0);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "Failed to send the final part of the page of length 0");
		return httpd_resp_send_500(req);
	}

    return ESP_OK;
}

httpd_uri_t home_page = {
    .uri     = "/",
    .method  = HTTP_GET,
    .handler = home_handler,
};
