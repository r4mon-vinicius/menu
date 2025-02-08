add_library(joystick_led joystick_led.c)

# Adiciona o diretório da biblioteca como include para outros módulos
target_include_directories(joystick_led PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Linka com o Pico SDK e as outras bibliotecas
target_link_libraries(joystick_led
    pico_stdlib
    hardware_adc
    hardware_pwm
)
