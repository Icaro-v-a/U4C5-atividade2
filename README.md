# Atividade 1: Temporizador periódico.

Este projeto implementa um semáforo com temporização de 3 segundos para cada alteração de sinal

## Descrição do Projeto

Nesta prática, será necessário simular os seguintes componentes:

1) Microcontrolador Raspberry Pi Pico W.
2) 03 LEDs (vermelho, amarelo e verde).
3) 03 Resistores de 330 Ω

### Requisitos do Projeto

1) O acionamento dos LEDs (sinais do semáforo) deve iniciar na  cor vermelha, alterando para amarela e, em seguida, verde.
2) O temporizador deve ser ajustado para um atraso de 3 segundos (3.000ms).
3) A mudança de estado dos LEDs deve ser implementa na função de call-back do temporizador, a exemplo da rotina trabalhada na aula síncrona - **repeating_timer_callback()**.
4) A rotina principal, presente no interior da estrutura de repetição while, deve imprimir algum tipo de informação a cada segundo (1.000 ms) - a mensagem enviada pela porta serial fica a critério do discente.
5) Com o emprego da Ferramenta Educacional BitDogLab, faça um experimento com o código deste exercício utilizando o LED RGB – GPIOs 11, 12 e 13.

## Tecnologias Utilizadas
- **Microcontrolador:** Raspberry Pi Pico W
- **Ambiente de Desenvolvimento:** VS Code
- **Simulador:** Wokwi
- **Linguagem:** C (com Pico SDK)

## Autor: Ícaro Vasconcelos Alvim

## Como Executar
1. Clone o repositório:
   ```bash
   git clone https://github.com/Icaro-v-a/U4C5-atividade1.git

2. Navegue até o diretório do projeto:
    ```bash
        cd U4C5-atividade1

3. Compile e execute o projeto utilizando o Pico SDK no VS Code.
