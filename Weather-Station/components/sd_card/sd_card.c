#include "sd_card.h"

#define TAG "SD_CARD_COMPONENT"
char const mount_point[] = SD_CARD_MOUNTPOINT;
#define mount_point_len 7
bool was_inited = false;
sdmmc_card_t * card_info;


esp_err_t sd_card_init(spi_bus_config_t const * sd_card_spi_config, gpio_num_t sd_card_cs_pin)
{
    esp_err_t result = ESP_OK;

    // Options for mounting the filesystem. 
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,    // Will NOT format the sd card if mount fails
        .max_files = 5,                     // Max nr of files that can be opened
        .allocation_unit_size = 16 * 1024,
        .disk_status_check_enable = true,   // Periodically checks sd card status
    };
    ESP_LOGD(TAG, "The inserted SD card will %s be formatted if mounting it fail", mount_config.format_if_mount_failed ? "" : "not");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();

    result = spi_bus_initialize(host.slot, sd_card_spi_config, SDSPI_DEFAULT_DMA);
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize the SPI bus to the SD card, because: ");
        if (result == ESP_ERR_INVALID_ARG) {
            ESP_LOGE(TAG, "the spi_bus_config_t configuration is invalid");
        } else if (result == ESP_ERR_INVALID_STATE) {
            ESP_LOGE(TAG, "the host is already in use");
        } else if (result == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "there is no available DMA channel");
        } else {
            ESP_LOGE(TAG, "the device is out of memory");
        }
        return ESP_ERR_INVALID_ARG;
    }
    ESP_LOGV(TAG, "Succesfully initialized the SPI bus to the SD card");

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = sd_card_cs_pin;
    slot_config.host_id = host.slot;

    result = esp_vfs_fat_sdspi_mount(
        mount_point, 
        &host, 
        &slot_config, 
        &mount_config, 
        &card_info
    );
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount the FAT filesystem on the SD card to the device, because: ");
        if (result == ESP_ERR_INVALID_STATE) {
            ESP_LOGE(TAG, "esp_vfs_fat_sdmmc_mount was already called");
        } else if (result == ESP_ERR_NO_MEM) {
            ESP_LOGE(TAG, "memory for it cannot be allocated");
        } else if (result == ESP_FAIL) {
            ESP_LOGE(TAG, "the partition cannot be mounted");
        } else {
            ESP_LOGE(TAG, "Error code was thrown from other drivers. IDK man?!");
        }
        return ESP_FAIL;
    }
    ESP_LOGV(TAG, "Succesfully mounted the filesystem from the SD card onto the device");

    was_inited = true;
    return ESP_OK;
}

// Parses given path to include the mount_point path of the sd card. 
// Returns heap allocated pointer w/ string that needs to be freed after use
char * parse_path(char const * path)
{
    int len = 0;
    bool include_dir_delimiter = false;

    while (path[len]) {
        len++;
    }

    len += mount_point_len;

    if (path[0] != '/') {
        len++;
        include_dir_delimiter = true;
    }

    char * new_path = (char *)malloc((len * sizeof(char)) + 1);

    int i = 0;
    int j = 0;

    while (mount_point[j]) {
        new_path[i] = mount_point[j];
        i++;
        j++;
    }

    if (include_dir_delimiter) {
        new_path[i] = '/';
        i++;
    }

    j = 0;
    while (path[j]) {
        new_path[i] = path[j];
        i++;
        j++;
    }

    new_path[i] = NULL;

    ESP_LOGV(TAG, "Got path: %s, and parsed it into: %s", path, new_path);

    return new_path;
}

esp_err_t sd_card_write(char const * path, char const * data, OPEN_MODE_t mode)
{
    esp_err_t result = sd_card_check();
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "SD card status check failed");
        return result;
    }

    if (was_inited == false) {
        ESP_LOGE(TAG, "Tried to write to the sd card bt the sd card was not initialized");
        return ESP_FAIL;
    }

    if (path == NULL || data == NULL) {
        if (path == NULL) {
            ESP_LOGE(TAG, "Got an empty path to the file to write");
        } else {
            ESP_LOGE(TAG, "sd_card_write was called with no data to write");
        }
        return ESP_ERR_INVALID_ARG;
    }

    FILE * file = NULL;
    char * parsed_path = parse_path(path);

    ESP_LOGD(TAG, "Opening file %s", parsed_path);
    switch (mode) {
        case OVERWRITE:
            file = fopen(parsed_path, "w");
            break;
    
        case APPEND:
        default:
            file = fopen(parsed_path, "a");
            break;
    }

    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file: %s for writing", parsed_path);
        free(parsed_path);
        return ESP_FAIL;
    }

    ESP_LOGD(TAG, "Writing data to file %s", parsed_path);
    fprintf(file, data);
    ESP_LOGD(TAG, "Closing file %s", parsed_path);
    fclose(file);
    free(parsed_path);

    return ESP_OK;
}

esp_err_t sd_card_file_exists(char const * filename)
{
    esp_err_t result = sd_card_check();
    if (result != ESP_OK) {
        ESP_LOGE(TAG, "SD card status check failed");
        return result;
    }

    if (was_inited == false) {
        ESP_LOGE(TAG, "Tried to check if a file exists, but the sd card was not initialized");
        return ESP_FAIL;
    }

    struct stat file_stat;
    char const * path = parse_path(filename);
    
    if (stat(path, &file_stat) == -1) {
        result = ESP_FAIL;
    }

    free((void *)path);
    return result;
}

FILE * sd_card_fopen(char const * filename, char const * modes)
{
    char const * path = parse_path(filename);
    
    FILE * file = fopen(path, modes);
    
    free((void *)path);
    
    return file;
}

esp_err_t sd_card_check()
{
    esp_err_t result = sdmmc_get_status(card_info);

    return result;
}
