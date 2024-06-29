
set(src_dirs    ai
                camera
                lcd
                )

set(include_dirs    ai
                    camera
                    lcd)

set(requires    esp-dl
                esp_http_server
                nvs_flash
                esp_adc
                esp_lcd
                esp_timer
                fb_gfx)

idf_component_register(SRC_DIRS ${src_dirs} INCLUDE_DIRS ${include_dirs} REQUIRES ${requires} EMBED_FILES ${embed_files})

component_compile_options(-ffast-math -O3 -Wno-error=format=-Wno-format)
