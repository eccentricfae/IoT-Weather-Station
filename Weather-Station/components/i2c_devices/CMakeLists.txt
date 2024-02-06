idf_component_register(
    SRCS 
    "i2c_devices.c"
    
    INCLUDE_DIRS 
    "include"
    
    REQUIRES 
    am2320
    bmp280
    hd44780
    pcf8574
    helpers
)