file(GLOB parsed_srcs ${CMAKE_CURRENT_SOURCE_DIR}/pages/*.c)

if (NOT CMAKE_BUILD_EARLY_EXPANSION)
    # Generate the .c file w/ parsed (into C from HTML / CSS / JS) source code for the webpages 
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_SOURCE_DIR}/pages/fake.c # Needs a fake to trick cmake to always run this command
            ${parsed_srcs}

        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/parser.pyp
    )
endif()

idf_component_register(
    SRCS 
    "server.c"
    
    INCLUDE_DIRS 
    "include"
    
    REQUIRES
    esp_http_server
    wifi
    settings_types
    helpers
    i2c_devices
    sd_card
    cJSON
)
