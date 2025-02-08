#include "led_rgb.h"

const uint LED = 12;
const uint16_t PER = 2000;
const float DIV_PWM = 16.0;
const uint16_t LED_STEP = 100;
uint16_t led_level = 100;
uint64_t last_update_time = 0;

void setup_pwm() {
    uint slice;
    gpio_set_function(LED, GPIO_FUNC_PWM);
    slice = pwm_gpio_to_slice_num(LED);
    pwm_set_clkdiv(slice, DIV_PWM);
    pwm_set_wrap(slice, PER);
    pwm_set_gpio_level(LED, led_level);
    pwm_set_enabled(slice, true);
}

void led_rgb() {
    uint up_down = 1;
    setup_pwm();
    last_update_time = time_us_64(); // Armazena o tempo inicial

    while (in_function) {
        uint64_t current_time = time_us_64(); // Obtém o tempo atual

        if (current_time - last_update_time >= 1000000) { // A cada 1 segundo atualiza o nível do LED
            last_update_time += 1000000; // Atualiza o tempo de referência

            // Atualiza o nível do LED
            pwm_set_gpio_level(LED, led_level);

            if (up_down) {
                led_level += LED_STEP;
                if (led_level >= PER)
                    up_down = 0;
            } else {
                led_level -= LED_STEP;
                if (led_level <= LED_STEP)
                    up_down = 1;
            }
        }
    }

    pwm_set_gpio_level(LED, 0);
    led_level = 100;
}