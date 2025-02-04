#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos pinos GPIO dos LEDs e botão
#define LED_BLUE 11
#define LED_RED 12
#define LED_GREEN 13
#define PUSH_BUTTON 5

// Variáveis globais
volatile bool em_execucao = false; // Impede múltiplas execuções simultâneas
volatile int estado = 0;            // Estado do semáforo
alarm_id_t timer_id;                // ID do temporizador

// Protótipos das funções
void led_init(void);
void btn_init(void);
void iniciar_sequencia(void);
int64_t mudar_estado_callback(alarm_id_t id, void *user_data);
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

// Inicializa o botão
void btn_init()
{
    gpio_init(PUSH_BUTTON);
    gpio_set_dir(PUSH_BUTTON, GPIO_IN);
    gpio_pull_up(PUSH_BUTTON);
}

// Aguarda até que o botão seja solto
void aguardar_soltura(void)
{
    while (gpio_get(PUSH_BUTTON) == 1)
    {
        sleep_ms(10); // Pequeno delay para evitar loop travado
    }
}

// Inicia a sequência quando o botão for pressionado
void iniciar_sequencia(void)
{
    printf("Executando sequência de LEDs...\n");
    em_execucao = true;
    estado = 0;

    // Liga todos os LEDs inicialmente
    gpio_put(LED_BLUE, 1);
    gpio_put(LED_RED, 1);
    gpio_put(LED_GREEN, 1);

    // Inicia o temporizador para alternar os estados a cada 3 segundos
    timer_id = add_alarm_in_ms(3000, mudar_estado_callback, NULL, false);
}

// Callback para alternar os estados dos LEDs
int64_t mudar_estado_callback(alarm_id_t id, void *user_data)
{
    estado++;

    switch (estado)
    {
    case 1:
        printf("Estado 1: Apenas vermelho e verde\n");
        gpio_put(LED_BLUE, 0);
        break;
    case 2:
        printf("Estado 2: Apenas verde\n");
        gpio_put(LED_RED, 0);
        break;
    case 3:
        printf("Estado 3: Todos apagados\n");
        gpio_put(LED_GREEN, 0);
        em_execucao = false;
        printf("Sequência concluída. Aguardando novo clique.\n");
        return 0; // Finaliza o temporizador
    default:
        return 0;
    }

    // Reagenda o temporizador para continuar a sequência
    timer_id = add_alarm_in_ms(3000, mudar_estado_callback, NULL, false);
    return 0;
}

int main(void)
{
    stdio_init_all();
    led_init();
    btn_init();

    printf("Sistema inicializado. Aguardando botão...\n");

    while (true)
    {
        if (gpio_get(PUSH_BUTTON) == 1 && !em_execucao)
        {
            sleep_ms(100); // Debounce
            if (gpio_get(PUSH_BUTTON) == 1) // Confirma que ainda está pressionado
            {
                aguardar_soltura(); // Espera o botão ser solto antes de continuar
                printf("Botão pressionado. Iniciando sequência...\n");
                iniciar_sequencia();
            }
        }
    }
}
