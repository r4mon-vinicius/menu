#include "buzzer.h"

const uint star_wars_notes[] = {
    330, 330, 330, 262, 392, 523, 330, 262,
    392, 523, 330, 659, 659, 659, 698, 523,
    415, 349, 330, 262, 392, 523, 330, 262,
    392, 523, 330, 659, 659, 659, 698, 523,
    415, 349, 330, 523, 494, 440, 392, 330,
    659, 784, 659, 523, 494, 440, 392, 330,
    659, 659, 330, 784, 880, 698, 784, 659,
    523, 494, 440, 392, 659, 784, 659, 523,
    494, 440, 392, 330, 659, 523, 659, 262,
    330, 294, 247, 262, 220, 262, 330, 262,
    330, 294, 247, 262, 330, 392, 523, 440,
    349, 330, 659, 784, 659, 523, 494, 440,
    392, 659, 784, 659, 523, 494, 440, 392
};

// Duração das notas em milissegundos
const uint note_duration[] = {
    500, 500, 500, 350, 150, 300, 500, 350,
    150, 300, 500, 500, 500, 500, 350, 150,
    300, 500, 500, 350, 150, 300, 500, 350,
    150, 300, 650, 500, 150, 300, 500, 350,
    150, 300, 500, 150, 300, 500, 350, 150,
    300, 650, 500, 350, 150, 300, 500, 350,
    150, 300, 500, 500, 500, 500, 350, 150,
    300, 500, 500, 350, 150, 300, 500, 350,
    150, 300, 500, 350, 150, 300, 500, 500,
    350, 150, 300, 500, 500, 350, 150, 300,
};

uint64_t note_start_time = 0;
int current_note = 0;
bool in_pause = false;  // Indica se está na pausa

// Inicializa o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f); // Ajusta divisor de clock
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0); // Desliga o PWM inicialmente
}

// Define a frequência da nota no PWM
void set_tone(uint pin, uint frequency) {
    if (frequency == 0) {
        pwm_set_gpio_level(pin, 0); // Silencia o buzzer
        return;
    }

    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / frequency - 1;

    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(pin, top / 2); // 50% de duty cycle
}

// Toca a música de Star Wars no buzzer
void play_star_wars(uint pin) {
    uint64_t current_time = time_us_64() / 1000; // Tempo atual em ms

    if (current_note >= sizeof(star_wars_notes) / sizeof(star_wars_notes[0])) {
        current_note = 0;  // Reinicia a música
    }

    if (!in_pause) {  // Se não estiver em pausa, toca a nota atual
        if (current_time - note_start_time >= note_duration[current_note]) {
            pwm_set_gpio_level(pin, 0); // Desliga a nota para a pausa
            note_start_time = current_time;
            in_pause = true;
        }
    } else {  // Se estiver em pausa, aguarda 50 ms antes da próxima nota
        if (current_time - note_start_time >= 50) {
            current_note++;
            if (current_note < sizeof(star_wars_notes) / sizeof(star_wars_notes[0])) {
                set_tone(pin, star_wars_notes[current_note]);
            }
            note_start_time = current_time;
            in_pause = false;
        }
    }
}

void buzzer() {
    current_note = 0;
    note_start_time = time_us_64() / 1000;
    in_pause = false;

    pwm_init_buzzer(BUZZER_PIN);
    
    while (in_function) {
        play_star_wars(BUZZER_PIN);
        sleep_ms(1); // Pequeno atraso 
    }
    
    pwm_set_gpio_level(BUZZER_PIN, 0); // Desliga o buzzer
}
