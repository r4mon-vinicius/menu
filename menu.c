#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "ssd1306.h"
#include "joystick_led.h"
#include "buzzer.h"
#include "led_rgb.h"

// Define os pinos do I2C e joystick
#define I2C_SDA 14
#define I2C_SCL 15
#define JOYSTICK_Y 26 // ADC0 para eixo Y do joystick
#define JOYSTICK_X 27 // ADC1 para eixo X do joystick
#define JOYSTICK_BTN 22 // Botão do joystick

// Define os limites do joystick
#define JOYSTICK_THRESHOLD_UP 4000   // Para cima
#define JOYSTICK_THRESHOLD_DOWN 1000 // Para baixo

#define DEBOUNCE_TIME_MS 200 // Tempo de debounce para o botão

volatile uint32_t last_button_press = 0;  // Último acionamento do botão (Debounce)

volatile int menu_pos = 0; // Variável de controle do menu
volatile bool function_selected = false; // Flag que indica se uma função foi selecionada
volatile int current_function = -1; // Indica a função atual
volatile bool in_function = false; // Indica se uma função está sendo executada

char *menu_options[] = {" JOYSTICK LED", " BUZZER", " LED RGB"}; // Opções do menu
#define MENU_SIZE (sizeof(menu_options) / sizeof(menu_options[0])) // Quantidade de opções do menu

// Protótipos de funções
void init_display(uint8_t *ssd, struct render_area *frame_area);
void draw_menu(uint8_t *ssd, struct render_area frame_area, int selected);
int read_joystick();
void button_callback(uint gpio, uint32_t events);

// Vetor de ponteiros para as funções do menu
void (*menu_functions[])() = {joystick_led, buzzer, led_rgb};

int main() {
    stdio_init_all(); // Habilita comunicação serial

    // Inicialização do display
    uint8_t ssd[ssd1306_buffer_length];
    struct render_area frame_area;
    init_display(ssd, &frame_area);

    // Inicialização do ADC
    adc_init();
    adc_gpio_init(JOYSTICK_Y);
    adc_gpio_init(JOYSTICK_X);

    // Configuração do botão do joystick
    gpio_init(JOYSTICK_BTN);
    gpio_set_dir(JOYSTICK_BTN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BTN); // Pull-up interno
    gpio_set_irq_enabled_with_callback(JOYSTICK_BTN, GPIO_IRQ_EDGE_FALL, true, &button_callback); 

    // Desenha o menu
    draw_menu(ssd, frame_area, menu_pos);

    while (true) {
        if (!function_selected) { // Só processa o menu se nenhuma função estiver ativa
            int new_pos = read_joystick();
            if (new_pos != menu_pos) {
                menu_pos = new_pos;
                draw_menu(ssd, frame_area, menu_pos);
            }
        } else { // Executa a função escolhida
            in_function = true; // Indica que está em execução
            menu_functions[current_function](); // Chama a função correspondente
            in_function = false; // Volta ao menu ao pressionar o botão
            function_selected = false; // Permite navegar no menu novamente
            draw_menu(ssd, frame_area, menu_pos); // Redesenha o menu ao sair
        }

        sleep_ms(100);
    }
}

// Inicializa o display OLED
void init_display(uint8_t *ssd, struct render_area *frame_area) {
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();
    *frame_area = (struct render_area) {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(frame_area);
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, frame_area);
}

// Callback para o botão do joystick
void button_callback(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time()); // Obtém tempo atual

    // Se o tempo desde a última ativação for menor que o tempo de debounce, ignora
    if (current_time - last_button_press < DEBOUNCE_TIME_MS) {
        return;
    }
    last_button_press = current_time; // Atualiza o tempo do último acionamento

    // Lógica do botão
    if (gpio == JOYSTICK_BTN) {
        if (in_function) {
            in_function = false; // Se estiver em uma função, voltar ao menu
        } else if (!function_selected) {
            function_selected = true; // Se estiver no menu, seleciona a função
            current_function = menu_pos;
        }
    }
}

// Desenha o menu no OLED
void draw_menu(uint8_t *ssd, struct render_area frame_area, int selected) {
    memset(ssd, 0, ssd1306_buffer_length); // Limpa a tela

    for (int i = 0; i < MENU_SIZE; i++) {
        if (i == selected) {
            ssd1306_draw_string(ssd, 0, 12 + i * 16, "x"); // Desenha o marcador ('x')
        }
        ssd1306_draw_string(ssd, 10, 12 + i * 16, menu_options[i]); // Desenha a opção
    }

    render_on_display(ssd, &frame_area);
}

// Lê o joystick e ajusta a posição do menu
int read_joystick() {
    adc_select_input(0); // Seleciona ADC0 (Y do joystick)
    uint16_t value = adc_read(); // Lê o valor do ADC

    if (value > JOYSTICK_THRESHOLD_UP) {
        return (menu_pos > 0) ? menu_pos - 1 : menu_pos; // Sobe no menu
    } else if (value < JOYSTICK_THRESHOLD_DOWN) {
        return (menu_pos < MENU_SIZE - 1) ? menu_pos + 1 : menu_pos; // Desce no menu
    }
    return menu_pos;
}
