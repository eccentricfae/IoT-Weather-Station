idf_component_register(
    SRCS 
    "Weather-Station.c"
    
    INCLUDE_DIRS 
    "."
    
    REQUIRES 
    server 
    wifi    
    helpers
    sd_card
    i2c_devices
    settings_types
    esp_event
    nvs_flash
    driver
    esp_timer
)
