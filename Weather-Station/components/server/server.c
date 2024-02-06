#include "server.h"
#include "pages/home.c"
#include "pages/device_info.c"
#include "pages/settings.c"
#include "rest_api/data.c"
#include "rest_api/files.c"

#define TAG "SERVER_COMPONENT"

httpd_handle_t server_handle = NULL;

esp_err_t register_page_helper(httpd_uri_t * page);

esp_err_t server_init(data_aggregate_t * l_data_for_server)
{
    httpd_config_t server_config = HTTPD_DEFAULT_CONFIG();

    if (l_data_for_server == NULL) {
        ESP_LOGE(TAG, "Passed NULL pointer to the data_aggregate_t needed for the server");
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t result = ESP_OK;
    
    result = httpd_start(&server_handle, &server_config);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start the httpd service (server), because: ");
        if (result == ESP_ERR_INVALID_ARG) {
            ESP_LOGE(TAG, "Passed null arguments");
        } else if (result == ESP_ERR_HTTPD_ALLOC_MEM) {
            ESP_LOGE(TAG, "Failed to allocate memory for the httpd instance");
        } else if (result == ESP_ERR_HTTPD_TASK) {
            ESP_LOGE(TAG, "Failed to launch server task");
        }
        return ESP_FAIL;
    }

    home_page_device_data = l_data_for_server;
    result = register_page_helper(&home_page);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add the \"home\" page uri handler");
        return result;
    }

    device_info_page_device_data = l_data_for_server;
    result = register_page_helper(&device_info_page);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add the \"device info\" page uri handler");
        return result;
    }

    settings_page_device_data = l_data_for_server;
    result = register_page_helper(&settings_page);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add the \"settings\" page uri handler");
        return result;
    }
    result = register_page_helper(&settings_post_page);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add the \"settings\" page POST uri handler");
        return result;
    }

    restful_data_data = l_data_for_server;
    result = register_page_helper(&restful_data_current_sensors);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add the \"current_sensors\" restful uri handler");
        return result;
    }
    result = register_page_helper(&restful_data_settings);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add the \"settings\" restful uri handler");
        return result;
    }
    
    restful_files_data = l_data_for_server;
    result = register_page_helper(&restful_files_download);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add the \"file download\" restful uri handler");
        return result;
    }
    result = register_page_helper(&restful_files_list);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add the \"files list\" restful uri handler");
        return result;
    }

    return ESP_OK;
}

esp_err_t register_page_helper(httpd_uri_t * page)
{
    esp_err_t result = httpd_register_uri_handler(server_handle, page);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register %s URI handler, because: ", page->uri);
        if (result == ESP_ERR_INVALID_ARG) {
            ESP_LOGE(TAG, "passed null arguments");
        } else if (result == ESP_ERR_HTTPD_HANDLERS_FULL) {
            ESP_LOGE(TAG, "there are no empty slots left for new handlers");
        } else {
            ESP_LOGE(TAG, "this handler is already registered");
        }
    }

    return result;
}
