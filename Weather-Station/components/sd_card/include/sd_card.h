#ifndef SD_CARD_H
#define SD_CARD_H

#include <esp_log.h>
#include <esp_vfs_fat.h>
#include <sdmmc_cmd.h>
#include <driver/spi_common.h>
#include <string.h>
#include <sys/stat.h>
#include "cJSON.h"
#include "settings_types.h"

#define SD_CARD_MOUNTPOINT "/sdcard"

/**
 * @brief Enum for specifying the mode with which to open the file for writing onto the sd card
 */
typedef enum WRITE_MODE {
    OVERWRITE,
    APPEND,
    READ,
} OPEN_MODE_t;

/**
 * @brief Initializes the SD card, by: 1) initializing the SPI bus to the card, 
 * 2) Mounting the FAt filesystem from the SD card onto the device
 * 
 * @param sd_card_spi_config SPI bus config to the SD card
 * @param sd_card_cs_pin CS pin of the device connected to the CS pin of the SD card's SPI bus
 * @return esp_err_t ESP_OK if succesfull, or error code from IDF's functions. Check logs
 */
esp_err_t sd_card_init(spi_bus_config_t const * sd_card_spi_config, gpio_num_t sd_card_cs_pin);

/**
 * @brief Writes given data (by data argument) to the given file (by path argument)
 * on the mounted SD card. If the SD card was not mounted by using the sd_card_init()
 * function first, the behaviour is undefined
 * @param path Path to the file on the SD card to be written into. Can start with '/' or not
 * @note If the path is NOT in the SD card base directory, the directory for the file
 * needs to be created or already exist, otherwise it'll fail
 * @param data Data to be written
 * @param mode Mode for opening the file
 * @return esp_err_t ESP_OK if succesfull or error code (check logs)
 */
esp_err_t sd_card_write(char const * path, char const * data, OPEN_MODE_t mode);

/**
 * @brief Reads the configuration settings from the SD card's file "settings.json" 
 * and stores it into the basic_settings_t struct pointed to by the argument
 * @param out_settings Pointer to the struct where the settings will be stored
 * @note The function dynamically allocates memory for the settings. After use
 * the free_settings() function should be called
 * @return esp_err_t ESP_OK if succesfull or an erorr code (check logs)
 */
esp_err_t sd_card_read_settings(basic_settings_t * out_settings);

/**
 * @brief Given configuration settings by the argument, the function writes
 * the settings to the "settings.json" file onto the SD card
 * @param in_settings Pointer to the struct with the settings to be written
 * @return esp_err_t ESP_OK if succesfull or an error code (check logs)
 */
esp_err_t sd_card_write_settings(basic_settings_t const * in_settings);

/**
 * @brief Checks if a file given by the name filename exists on the sd card
 * @param filename [in] Name of the file
 * @return esp_err_t ESP_OK if file exists
 */
esp_err_t sd_card_file_exists(char const * filename);

/**
 * @brief Wrapper for the fopen function
 * @param filename [in] Name of the file
 * @return FILE* Pointer to the opened file or NULL if failed
 */
FILE * sd_card_fopen(char const * filename, char const * modes);

/**
 * @brief Checks the status (if the card is still inserted)
 * @return esp_err_t ESP_OK if OK, error otherwise
 */
esp_err_t sd_card_check();


#endif