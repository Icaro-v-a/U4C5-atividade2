#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos LEDs
#define LED_RED 11
#define LED_YELLOW 12
#define LED_GREEN 13

// Variável global para armazenar o estado do semáforo
volatile int estado = 0;

// Protótipos das funções
bool semaforo_callback(struct repeating_timer *t);
void led_init(void);

void led_init()
{
    // Configuração inicial dos LEDs
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_put(LED_RED, 0);

    gpio_init(LED_YELLOW);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_put(LED_YELLOW, 0);

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_put(LED_GREEN, 0);
}

// Callback para alternar os LEDs
bool semaforo_callback(struct repeating_timer *t)
{
    gpio_put(LED_RED, 0);
    gpio_put(LED_YELLOW, 0);
    gpio_put(LED_GREEN, 0);

    if (estado == 0)
    {
        gpio_put(LED_RED, 1);
        estado = 1;
    }
    else if (estado == 1)
    {
        gpio_put(LED_YELLOW, 1);
        estado = 2;
    }
    else
    {
        gpio_put(LED_GREEN, 1);
        estado = 0;
    }

    return true;
}

int main()
{
    stdio_init_all();
    led_init();

    // Acende o LED vermelho imediatamente ao iniciar
    gpio_put(LED_RED, 1);
    estado = 1; // Para garantir que o próximo será o amarelo

    struct repeating_timer timer_semaforo;

    // Inicia o temporizador para mudar os LEDs a cada 3 segundos
    add_repeating_timer_ms(3000, semaforo_callback, NULL, &timer_semaforo);

    // Variável para controle do tempo da impressão serial
    uint64_t tempo_anterior = time_us_64();

    while (true)
    {
        uint64_t tempo_atual = time_us_64();

        // Verifica se passou 1 segundo (1.000.000 microssegundos)
        if ((tempo_atual - tempo_anterior) >= 1000000)
        {
            printf("Semáforo operando...\n");
            tempo_anterior = tempo_atual;
        }

        tight_loop_contents(); // Mantém o loop eficiente
    }
}
