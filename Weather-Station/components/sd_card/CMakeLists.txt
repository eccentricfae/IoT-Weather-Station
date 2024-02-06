idf_component_register(
    SRCS 
    "sd_card.c"
    "parsing_settings.c"
    
    INCLUDE_DIRS 
    "include"
    
    REQUIRES 
    fatfs
    driver
    sdmmc
    settings_types
    cJSON
)