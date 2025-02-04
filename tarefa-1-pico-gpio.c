#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Definição dos pinos GPIO dos LEDs e botão
#define LED_BLUE 11      // Pino para o LED azul
#define LED_RED 12       // Pino para o LED vermelho
#define LED_GREEN 13     // Pino para o LED verde
#define PUSH_BUTTON 5    // Pino para o botão

// Variáveis globais
volatile bool em_execucao = false;  // Flag que indica se a sequência está em execução
volatile int estado = 0;            // Armazena o estado atual do semáforo (sequência de LEDs)
alarm_id_t timer_id;                // ID do temporizador que controla a alternância dos estados

// Protótipos das funções
void led_init(void);
void btn_init(void);
void iniciar_sequencia(void);
int64_t mudar_estado_callback(alarm_id_t id, void *user_data);
void aguardar_soltura(void);

// Inicializa os LEDs: configura os pinos para saída e os apaga inicialmente
void led_init()
{
    gpio_init(LED_BLUE);          // Inicializa o pino do LED azul
    gpio_set_dir(LED_BLUE, GPIO_OUT);  // Define o pino como saída
    gpio_put(LED_BLUE, 0);        // Apaga o LED azul

    gpio_init(LED_RED);           // Inicializa o pino do LED vermelho
    gpio_set_dir(LED_RED, GPIO_OUT);   // Define o pino como saída
    gpio_put(LED_RED, 0);         // Apaga o LED vermelho

    gpio_init(LED_GREEN);         // Inicializa o pino do LED verde
    gpio_set_dir(LED_GREEN, GPIO_OUT); // Define o pino como saída
    gpio_put(LED_GREEN, 0);       // Apaga o LED verde
}

// Inicializa o botão: configura o pino como entrada com pull-up interno
void btn_init()
{
    gpio_init(PUSH_BUTTON);          // Inicializa o pino do botão
    gpio_set_dir(PUSH_BUTTON, GPIO_IN);  // Define o pino como entrada
    gpio_pull_up(PUSH_BUTTON);       // Ativa o resistor de pull-up no pino
}

// Aguarda até que o botão seja solto
void aguardar_soltura(void)
{
    // Enquanto o botão estiver pressionado (nível lógico baixo)
    while (gpio_get(PUSH_BUTTON) == 1)
    {
        sleep_ms(10); // Pequeno delay de 10 ms para evitar loop travado
    }
}

// Inicia a sequência de LEDs quando o botão for pressionado
void iniciar_sequencia(void)
{
    printf("Executando sequência de LEDs...\n");
    em_execucao = true; // Marca a sequência como em execução
    estado = 0;         // Reinicia o estado do semáforo (LEDs)

    // Liga todos os LEDs inicialmente
    gpio_put(LED_BLUE, 1);   // Liga o LED azul
    gpio_put(LED_RED, 1);    // Liga o LED vermelho
    gpio_put(LED_GREEN, 1);  // Liga o LED verde

    // Inicia o temporizador para alternar os estados a cada 3 segundos
    timer_id = add_alarm_in_ms(3000, mudar_estado_callback, NULL, false);
}

// Callback que alterna os estados dos LEDs após cada intervalo de tempo
int64_t mudar_estado_callback(alarm_id_t id, void *user_data)
{
    estado++;  // Incrementa o estado a cada chamada

    switch (estado)
    {
    case 1:
        // Estado 1: Apaga o LED azul e deixa os outros acesos
        printf("Estado 1: Apenas vermelho e verde\n");
        gpio_put(LED_BLUE, 0);  // Desliga o LED azul
        break;
    case 2:
        // Estado 2: Apaga o LED vermelho, deixando o verde aceso
        printf("Estado 2: Apenas verde\n");
        gpio_put(LED_RED, 0);   // Desliga o LED vermelho
        break;
    case 3:
        // Estado 3: Apaga todos os LEDs
        printf("Estado 3: Todos apagados\n");
        gpio_put(LED_GREEN, 0); // Desliga o LED verde
        em_execucao = false;    // Marca a execução como finalizada
        printf("Sequência concluída. Aguardando novo clique.\n");
        return 0; // Finaliza o temporizador
    default:
        return 0; // Se o estado for desconhecido, finaliza o temporizador
    }

    // Reagenda o temporizador para continuar a sequência após 3 segundos
    timer_id = add_alarm_in_ms(3000, mudar_estado_callback, NULL, false);
    return 0;
}

int main(void)
{
    stdio_init_all();  // Inicializa a comunicação via UART para imprimir mensagens
    led_init();        // Inicializa os LEDs
    btn_init();        // Inicializa o botão

    printf("Sistema inicializado. Aguardando botão...\n");

    while (true)
    {
        // Se o botão for pressionado e a sequência não estiver em execução
        if (gpio_get(PUSH_BUTTON) == 1 && !em_execucao)
        {
            sleep_ms(100); // Debounce: aguarda 100 ms para evitar leituras falsas
            if (gpio_get(PUSH_BUTTON) == 1) // Confirma que o botão ainda está pressionado
            {
                aguardar_soltura(); // Aguarda o botão ser solto antes de continuar
                printf("Botão pressionado. Iniciando sequência...\n");
                iniciar_sequencia(); // Inicia a sequência de LEDs
            }
        }
    }
}
