#ifndef JOYSTICK_LED_H
#define JOYSTICK_LED_H

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

// Definições dos pinos e configurações de PWM
#define LED_B 13
#define LED_R 11
#define DIVIDER_PWM 16.0
#define PERIOD 4096

extern volatile bool in_function;

// Inicializa os LEDs com PWM
void setup_pwm_led(uint led, uint *slice, uint16_t level);

// Lê os valores dos eixos do joystick
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value);

// Controla os LEDs com o joystick
void joystick_led();

#endif
