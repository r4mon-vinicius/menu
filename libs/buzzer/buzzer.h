#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define BUZZER_PIN 21  // Definição do pino do buzzer

extern volatile bool in_function; 

// Inicializa o buzzer
void pwm_init_buzzer(uint pin);

// Define a frequência do buzzer
void set_tone(uint pin, uint frequency);

// Toca a música do Star Wars
void play_star_wars(uint pin);

void buzzer();

#endif 
