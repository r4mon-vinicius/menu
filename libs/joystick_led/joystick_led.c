#include "joystick_led.h"

uint slice_led_b, slice_led_r;
uint16_t led_b_level = 100, led_r_level = 100;

void setup_pwm_led(uint led, uint *slice, uint16_t level) {
    gpio_set_function(led, GPIO_FUNC_PWM);
    *slice = pwm_gpio_to_slice_num(led);
    pwm_set_clkdiv(*slice, DIVIDER_PWM);
    pwm_set_wrap(*slice, PERIOD);
    pwm_set_gpio_level(led, level);
    pwm_set_enabled(*slice, true);
}

void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value) {
    adc_select_input(0);
    sleep_us(2);
    *vrx_value = adc_read();

    adc_select_input(1);
    sleep_us(2);
    *vry_value = adc_read();
}

void joystick_led() {
    uint16_t vrx_value, vry_value;

    setup_pwm_led(LED_B, &slice_led_b, led_b_level);
    setup_pwm_led(LED_R, &slice_led_r, led_r_level);

    while (in_function) {
        joystick_read_axis(&vrx_value, &vry_value);
        pwm_set_gpio_level(LED_B, vrx_value);
        pwm_set_gpio_level(LED_R, vry_value);
        sleep_ms(10);
    }

    pwm_set_gpio_level(LED_B, 0);
    pwm_set_gpio_level(LED_R, 0);
}
