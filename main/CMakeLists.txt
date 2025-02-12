idf_component_register(
    SRCS "controller_can.c" "controller_uart.c" "waveshare_rgb_lcd_port.c" "main.c" "lvgl_port.c" "model.c"
    INCLUDE_DIRS ".")

add_subdirectory(ui)
target_link_libraries(${COMPONENT_LIB} PRIVATE ui)

idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
target_compile_options(${lvgl_lib} PRIVATE -Wno-format)
