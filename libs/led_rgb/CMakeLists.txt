add_library(led_rgb led_rgb.c)

# Adiciona o diretório da biblioteca como include para outros módulos
target_include_directories(led_rgb PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Linka com o Pico SDK e as outras bibliotecas
target_link_libraries(led_rgb
    pico_stdlib
    pico_time
    hardware_pwm
)
