#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/pwm.h"


extern volatile bool in_function;

const uint LED = 12;            // Pino do LED conectado
const uint16_t PER = 2000;   // Período do PWM (valor máximo do contador)
const float DIV_PWM = 16.0; // Divisor fracional do clock para o PWM
const uint16_t LED_STEP = 100;  // Passo de incremento/decremento para o duty cycle do LED
uint16_t led_level = 100;       // Nível inicial do PWM (duty cycle)

void setup_pwm()
{
    uint slice;
    gpio_set_function(LED, GPIO_FUNC_PWM); // Configura o pino do LED para função PWM
    slice = pwm_gpio_to_slice_num(LED);    // Obtém o slice do PWM associado ao pino do LED
    pwm_set_clkdiv(slice, DIV_PWM);    // Define o divisor de clock do PWM
    pwm_set_wrap(slice, PER);           // Configura o valor máximo do contador (período do PWM)
    pwm_set_gpio_level(LED, led_level);    // Define o nível inicial do PWM para o pino do LED
    pwm_set_enabled(slice, true);          // Habilita o PWM no slice correspondente
}

void led_rgb()
{
    uint up_down = 1; // Variável para controlar se o nível do LED aumenta ou diminui
    setup_pwm();      // Configura o PWM
    while (in_function)
    {
        pwm_set_gpio_level(LED, led_level); // Define o nível atual do PWM (duty cycle)
        busy_wait_ms(1000);                     // Atraso de 1 segundo
        if (up_down)
        {
            led_level += LED_STEP; // Incrementa o nível do LED
            if (led_level >= PER)
                up_down = 0; // Muda direção para diminuir quando atingir o período máximo
        }
        else
        {
            led_level -= LED_STEP; // Decrementa o nível do LED
            if (led_level <= LED_STEP)
                up_down = 1; // Muda direção para aumentar quando atingir o mínimo
        }
    }
    pwm_set_gpio_level(LED, 0); // Desliga o LED ao sair da função
    led_level = 100;            // Restaura o nível inicial do PWM
}