Checklist de coisas para fazer

[] O motor do jogo - com ou sem renderização para que ocorra o treino
[X] Definir inputs
[X] Definir Outputs
[] Definir uma função de recompensa
[X] O algoritmo usado será o Deep-QLearning
[] Definir um passo a passo do algoritmo
[X] Definir a camada oculta e sistema de pesos
[X] Definir a Função de Erro

# Deep-QLearning

## Inputs
A ideia é um vetor com 11 entradas booleanas

- 3 entradas para definir obstaculos (parede ou o proprio corpo): [frente,esquerda,direita]. por exemplo: se tiver uma parede na esquerda e uma parte do corpo a frente o vetor será, [1,1,0];
- 4 entradas definindo a direção que a cobra está andando: [cima,baixo,esquerda,direita], esta entrada so pode assumir um valor por vez, então se a cobra estiver andando para a direita ela so pode assumir o valor [0,0,0,1];
- 4 entradas definindo a direção que a comida se encontra da cabeça da cobra, [cima,baixo,esquerda,direita], se X da comida for menor que o X da cabeça então [1,0], analogamente para o restante dos valores. Exemplo se a comida está acima e na direita da cabeça [1,0,0,1].

## Outputs
A saida é um vetor simples de 3 valores floats de variação de 0 a 1

- [cima,esquerda,direita], como não tem como virar para traz em um comando, então o comando de saída será de relativo a cabeça da cobra

## Camada Oculta
Uma camada com 256 neuronios (uma convenção entre programadores)

- Cada neuronios vai ter um vetor de pesos (floats) com valores aleatorios inicialmente e um vies (float) geralmente zerado.

## Função de Erro (Loss Function)
Mede o quão os resultados da rede está errada com base no valores de 'Q' gerados.


## Gradiente
Com o gradiente eu consigo analisar se o proximo valor medido de erro vai aumentar ou diminuir o erro, com base nos pontos criticos de um gradiente.

## Backpropagation
Apos algum erro o algoritmo analisa o neuronio que mais influenciou o resultado e tenta ajustar o peso

## Otimizador
Um pequeno ajuste nos pesos baseado na configuração Learning Rate.


## Fluxo

- Main organiza a chamada dos processos
- engine - o motor do jogo, trabalha a movimentação, colisões, tamanhos e por ai vai
- agent - para os dados de input para o agente treinar e retorna para a engine a saida
- Render - uma classe descartavel para apenas renderizar o resultado do jogo