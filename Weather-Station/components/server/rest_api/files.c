#include "server.h"

#define TAG "RESTFUL: FILES"

data_aggregate_t * restful_files_data = NULL;

esp_err_t restful_files_list_handler(httpd_req_t * req)
{
    esp_err_t result = ESP_OK;

    if (restful_files_data == NULL) {
        ESP_LOGE(TAG, "restful_files_data was not set!");
        return httpd_resp_send_500(req);
    }

    result = httpd_resp_set_type(req, "application/json");
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set request's response type");
        return httpd_resp_send_500(req);
    }

    result = httpd_resp_sendstr_chunk(
        req, 
        "{ \"files_list\": ["
    );
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send chunk of data");
        return httpd_resp_send_500(req);
    }

    char filename[300] = {0};
    struct dirent * dir_elem = NULL;
    bool first_elem = true;

    DIR * dir = opendir(SD_CARD_MOUNTPOINT);
    if (dir == NULL) {
        ESP_LOGE(TAG, "Failed to open the sd card's main directory");
        return httpd_resp_send_500(req);
    }
    
    while ((dir_elem = readdir(dir))) {
        if (dir_elem->d_type != DT_REG) {
            continue;
		}

        sprintf(filename, "%s\"%s\"", first_elem ? "" : ",", dir_elem->d_name);

        result = httpd_resp_sendstr_chunk(req, filename);
        if (result != ESP_OK) {
            ESP_LOGE(TAG, "Failed to send chunk of data");
            return httpd_resp_send_500(req);
        }

        memset(filename, 0, sizeof(filename));
        if (first_elem) {
            first_elem = false;
        }
    }

    result = httpd_resp_sendstr_chunk(
        req, 
        " ] }"
    );
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send chunk of data");
        return httpd_resp_send_500(req);
    }

    result = httpd_resp_send_chunk(req, NULL, 0);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "LIST: Failed to send last chunk of data");
        return httpd_resp_send_500(req);
    }

    return ESP_OK;
}

httpd_uri_t restful_files_list = {
    .uri     = "/api/files/list",
    .method  = HTTP_GET,
    .handler = restful_files_list_handler,
};

esp_err_t restful_files_download_handler(httpd_req_t * req)
{
    esp_err_t result = ESP_OK;

    if (restful_files_data == NULL) {
        ESP_LOGE(TAG, "restful_files_data was not set!");
        return httpd_resp_send_500(req);
    }

    size_t url_len = httpd_req_get_url_query_len(req);
    if (url_len == 0) {
        ESP_LOGE(TAG, "httpd_req_get_url_query_len failed?!");
        return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "The URL query requires key-value argument pair of key \"filename\" to select what file to download");
    } else if (url_len >= 25) {
        ESP_LOGE(TAG, "The URL query is longer than 25 characters. Should never be the case!");
        return httpd_resp_send_500(req);
    }

    char url[100] = {0};
    result = httpd_req_get_url_query_str(req, url, sizeof(url));
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get the URL query string");
        return httpd_resp_send_500(req);
    }

    char filename[14] = {0};

    result = httpd_query_key_value(url, "filename", filename, sizeof(filename));
    if (result != ESP_OK) {
        if (result == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "The filename key-value pair was not found in the URL query");
            return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "The URL query requires key-value argument pair of key \"filename\" to select what file to download");
        } else if (result == ESP_ERR_HTTPD_RESULT_TRUNC) {
            ESP_LOGE(TAG, "The filename value is not valid! Longer than 13 characters");
            return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "The filename value is too long. The filenames are always in format of ddmmyyyy.txt and are always of length 12");
        } else {
            ESP_LOGE(TAG, "Other error while getting key-value pair for filename");
            return httpd_resp_send_500(req);
        }
    }

    bool file_exists = sd_card_file_exists(filename) == ESP_OK;

    if (file_exists == false) {
        ESP_LOGE(TAG, "The file requested by the user was not found!");
        return httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "The requested file was not found!");
    }

    // (void) httpd_resp_set_type(req, "text/csv");
    (void) httpd_resp_set_type(req, "text/plain");

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

httpd_uri_t restful_files_download = {
    .uri     = "/api/files/download",
    .method  = HTTP_GET,
    .handler = restful_files_download_handler,
};
