#include <stdio.h>
#include "pico/stdlib.h"

// Definição dos pinos GPIO correspondentes aos LEDs e botão
#define LED_BLUE 11
#define LED_RED 12
#define LED_GREEN 13
#define PUSH_BUTTON 5

// Protótipos das funções
void led_init(void);
void btn_init(void);
void executar_sequencia(void);
void aguardar_soltura(void);

// Inicializa os LEDs
void led_init()
{
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_put(LED_BLUE, 0);

    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_put(LED_RED, 0);

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_put(LED_GREEN, 0);
}

// Inicializa o botão no Wokwi
void btn_init()
{
    gpio_init(PUSH_BUTTON);
    gpio_set_dir(PUSH_BUTTON, GPIO_IN);
    gpio_pull_up(PUSH_BUTTON); // Ativa resistor pull-up interno
}

// Aguarda até que o botão seja solto
void aguardar_soltura()
{
    while (gpio_get(PUSH_BUTTON) == 1)
    {
        sleep_ms(10); // Pequeno delay para evitar loop travado
    }
}

// Função para executar a sequência de LEDs
void executar_sequencia()
{
    printf("Executando sequência de LEDs...\n");

    // Estado 0: Todos os LEDs acesos
    gpio_put(LED_BLUE, 1);
    gpio_put(LED_RED, 1);
    gpio_put(LED_GREEN, 1);
    sleep_ms(3000);

    // Estado 1: Apenas vermelho e verde
    gpio_put(LED_BLUE, 0);
    sleep_ms(3000);

    // Estado 2: Apenas verde
    gpio_put(LED_RED, 0);
    sleep_ms(3000);

    // Estado 3: Todos apagados
    gpio_put(LED_GREEN, 0);
    printf("Sequência concluída. Aguardando novo clique.\n");
}

int main()
{
    stdio_init_all();
    led_init();
    btn_init();

    printf("Sistema inicializado. Aguardando botão...\n");

    while (true)
    {
        if (gpio_get(PUSH_BUTTON) == 1)
        {
            sleep_ms(100); // Debounce
            if (gpio_get(PUSH_BUTTON) == 1) // Confirma que ainda está pressionado
            {
                aguardar_soltura(); // Espera o botão ser solto antes de continuar
                printf("Botão pressionado. Iniciando sequência...\n");
                executar_sequencia();
            }
        }
    }
}
