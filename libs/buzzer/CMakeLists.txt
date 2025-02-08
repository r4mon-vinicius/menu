add_library(buzzer buzzer.c)

# Adiciona o diretório da biblioteca como include para outros módulos
target_include_directories(buzzer PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Linka com o Pico SDK e as outras bibliotecas
target_link_libraries(buzzer
    pico_stdlib
    hardware_clocks
    hardware_pwm
)
