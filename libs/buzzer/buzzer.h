#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define BUZZER_PIN 21  // Definição do pino do buzzer

extern volatile bool in_function; // Variável global externa

// Declaração das funções
void pwm_init_buzzer(uint pin);
void play_tone(uint pin, uint frequency, uint duration_ms);
void buzzer();

#endif 
