#ifndef LED_RGB_H
#define LED_RGB_H

#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/pwm.h"

extern volatile bool in_function;

// Configuração do PWM para controle do LED
void setup_pwm();

// Função que altera a intensidade do LED RGB
void led_rgb();

#endif 
