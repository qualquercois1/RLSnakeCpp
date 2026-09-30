# 🐍 Snake RL em C++ (Deep Q-Learning)

Projeto de aprendizado por reforço (*Reinforcement Learning*) onde uma cobra autônoma aprende a jogar o clássico jogo **Snake** utilizando o algoritmo **Deep Q-Learning (DQN)** implementado em **C++17** com renderização gráfica via **Raylib**.

---

## 📋 Checklist do Projeto

### 🎮 Motor do Jogo & Ambiente
- [x] Grade matricial customizável (`GRID_WIDTH`, `GRID_HEIGHT`, `CELL_SIZE`).
- [x] Lógica de movimentação em passos discretos e colisão (paredes e próprio corpo).
- [x] Spawn aleatório de maçã (nunca gerada dentro do corpo).
- [x] Sistema de movimentação relativo à cabeça (3 ações: frente, esquerda, direita).
- [x] Renderização gráfica opcional com Raylib (pode ser desativada para treino acelerado).
- [x] Modo de jogo manual para testes humanos com setas ou WASD.

### 🧠 Modelagem do Deep Q-Learning
- [x] **Vetor de Estado (Inputs)**: 11 valores booleanos/binários normalizados.
- [x] **Vetor de Ações (Outputs)**: 3 ações relativas à cabeça.
- [x] **Função de Recompensa (Reward Function)**:
  - Comer maçã: `+10.0`
  - Colisão / Game Over: `-10.0`
  - Passo normal: `-0.1` (incentiva trajetórias curtas).
- [x] Definição de hiperparâmetros no [`config.hpp`](file:///home/calcio/Documentos/Projetos/RLTrainSnakeCpp/config.hpp).

### 🤖 Agente & Rede Neural (Próximos Passos)
- [ ] Implementação da estrutura de Rede Neural (MLP / Perceptron Multicamadas):
  - Camada de entrada (11 neurônios).
  - Camada oculta (256 neurônios com ativação ReLU).
  - Camada de saída (3 neurônios com ativação linear para os Q-values).
- [ ] Algoritmo de *Forward Pass* (inferência).
- [ ] Algoritmo de *Backpropagation* e cálculo de gradientes.
- [ ] Otimizador (Gradient Descent com Learning Rate ou Adam).
- [ ] *Replay Memory* (armazenamento de tuplas `(s, a, r, s', done)` em deque circular).
- [ ] Política $\epsilon$-Greedy (decadência da taxa de exploração vs explotação).
- [ ] Treinamento em lotes (*Mini-Batch Train* e *Short-Term Train*).
- [ ] Persistência de pesos (salvar e carregar modelo treinado em arquivo).

---

## 🧩 Detalhamento do Deep Q-Learning (DQN)

O **Deep Q-Learning** combina o algoritmo clássico de Q-Learning com redes neurais artificiais. Em vez de usar uma tabela para guardar o valor de cada par `(estado, ação)`, utilizamos uma rede neural como aproximador de função que estima os **Q-Values**: $Q(s, a)$.

```
   [Estado s (11)]  ───►  [Rede Neural (256 ocultos)]  ───►  [Q-Values (3 saídas)]
                                                                  │
                                                        Maior valor = Ação escolhida
```

---

### 1. Entradas da Rede (Vetor de Estado - 11 Entradas)

O estado $s$ é representado por um vetor com **11 valores booleanos (0.0 ou 1.0)**, calculados no método [`Engine::getState()`](file:///home/calcio/Documentos/Projetos/RLTrainSnakeCpp/engine.cpp#L62):

| Índices | Descrição | Detalhes |
| :--- | :--- | :--- |
| **0, 1, 2** | **Perigo Imediato (Obstáculos)** | `[frente, esquerda, direita]` relativos à cabeça da cobra. Indica colisão iminente com a parede ou com o próprio corpo a 1 bloco de distância. |
| **3, 4, 5, 6** | **Direção Atual da Cobra** | *One-hot encoding* da direção absoluta: `[CIMA, BAIXO, ESQUERDA, DIREITA]`. Apenas um valor é `1.0` por vez. |
| **7, 8, 9, 10** | **Posição Relativa da Maçã** | `[Acima, Abaixo, À Esquerda, À Direita]`. Exemplo: se a comida estiver à direita e acima da cabeça, o vetor terá `[1, 0, 0, 1]`. |

---

### 2. Saídas da Rede (Vetor de Ações - 3 Comandos)

A saída é composta por **3 valores reais ($Q$-Values)**, correspondentes à recompensa futura estimada para cada ação relativa à cabeça da cobra:

| Índice | Ação | Descrição |
| :---: | :---: | :--- |
| `0` | **`ACTION_UP`** | Seguir em frente (mantém a direção atual). |
| `1` | **`ACTION_LEFT`** | Virar 90° à esquerda da perspectiva da cabeça. |
| `2` | **`ACTION_RIGHT`** | Virar 90° à direita da perspectiva da cabeça. |

> **Nota:** Como a cobra não pode reverter 180° instantaneamente para trás, 3 ações relativas são suficientes para cobrir todos os movimentos válidos possíveis.
> - Se a cobra estiver indo para **BAIXO** e escolher **ESQUERDA**, ela passará a se mover para a **DIREITA** na grade.
> - Se estiver indo para **BAIXO** e escolher **DIREITA**, passará a se mover para a **ESQUERDA**.

---

### 3. Função de Recompensa (Reward Function)

O agente aprende por tentativa e erro, recebendo feedbacks do ambiente a cada passo:
- **`+10.0` (Comer maçã):** Reforço positivo forte por atingir o objetivo principal.
- **`-10.0` (Colisão / Morte):** Punição severa ao bater nas paredes ou no próprio corpo.
- **`-0.1` (Passo normal):** Punição sutil a cada movimento. Isso desencoraja a cobra de ficar dando voltas infinitas sem ir em direção à comida, forçando-a a encontrar o caminho mais curto.

---

### 4. Arquitetura da Rede Neural

```
Camada de Entrada (11) ──► Camada Oculta (256) [ReLU] ──► Camada de Saída (3) [Linear]
```

- **Camada de Entrada ($X$):** 11 neurônios.
- **Camada Oculta ($H$):** 256 neurônios.
  - Matriz de pesos $W_1$ de dimensão $11 \times 256$.
  - Vetor de bias $b_1$ de dimensão $256$.
  - Ativação: **ReLU** ($f(x) = \max(0, x)$) para permitir aprendizado não-linear e evitar desaparecimento do gradiente.
- **Camada de Saída ($Y$):** 3 neurônios (um $Q$-value por ação).
  - Matriz de pesos $W_2$ de dimensão $256 \times 3$.
  - Vetor de bias $b_2$ de dimensão $3$.
  - Ativação: **Linear** (identidade), pois os $Q$-values podem ser negativos ou positivos.

---

### 5. Equação de Bellman e Alvo de Aprendizado

O objetivo da rede é aproximar a **Equação de Bellman**:

$$Q(s, a)_{target} = r + \gamma \cdot \max_{a'} Q(s', a')$$

- $r$: Recompensa imediata recebida após a ação $a$.
- $\gamma$ (`GAMMA = 0.90`): Fator de desconto para recompensas futuras (quanto mais próximo de 1, mais o agente valoriza o futuro a longo prazo).
- $s'$: Novo estado resultante da ação.
- Se o passo resultar em **Game Over**, não há futuro, então: $Q(s, a)_{target} = r$.

---

### 6. Função de Erro (Loss Function)

A diferença entre o valor predito pela rede $Q(s, a)_{pred}$ e o valor alvo da Equação de Bellman $Q(s, a)_{target}$ é calculada pelo **Erro Quadrático Médio (MSE - Mean Squared Error)**:

$$\text{Loss} = \frac{1}{2} \left( Q(s, a)_{target} - Q(s, a)_{pred} \right)^2$$

---

### 7. Gradiente Descendente e Backpropagation

1. **Forward Pass:** O estado $s$ entra na rede e gera os $Q$-values preditos.
2. **Cálculo da Perda:** Comparamos o $Q$-value da ação tomada com o $Q_{target}$ de Bellman.
3. **Backpropagation:** Propagamos a derivada do erro $\frac{\partial \text{Loss}}{\partial W}$ e $\frac{\partial \text{Loss}}{\partial b}$ pelas camadas da rede utilizando a regra da cadeia.
4. **Otimizador:** Atualizamos os pesos na direção oposta ao gradiente para minimizar o erro:
   $$W \leftarrow W - \alpha \cdot \frac{\partial \text{Loss}}{\partial W}$$
   onde $\alpha$ é o *Learning Rate* (`LEARNING_RATE = 0.001`).

---

### 8. Exploração vs Explotação ($\epsilon$-Greedy)

No início do treinamento, a rede possui pesos aleatórios e cometerá erros. Para explorar o mapa:
- Com probabilidade $\epsilon$ (*epsilon*), o agente toma uma **ação aleatória** (*Exploração*).
- Com probabilidade $1 - \epsilon$, o agente toma a **melhor ação segundo a rede** ($\arg\max Q$) (*Explotação*).
- O valor de $\epsilon$ diminui gradativamente ao longo dos primeiros $N$ jogos (`NUM_GAMES_TO_MAP = 80`), fazendo com que o agente dependa cada vez mais de sua própria inteligência.

---

### 9. Experience Replay (Memória de Experiências)

O agente armazena transições na memória `(s, a, r, s', done)`:
- **Short-term memory:** Treina a rede imediatamente a cada passo individual.
- **Long-term memory (Replay Buffer):** Ao fim de cada jogo ou periodicamente, sorteia um lote aleatório de transições (`BATCH_SIZE = 1000`) da memória acumulada (`MAX_MEMORY = 100000`). Isso remove correlações temporais sucessivas e estabiliza o treinamento da rede neural.

---

## 🏗️ Arquitetura do Código

```
RLTrainSnakeCpp/
├── config.hpp      # Hiperparâmetros de RL, física da grade e flags de execução
├── engine.hpp      # Declaração do motor do jogo, tipos (Point, Direction, Action)
├── engine.cpp      # Regras do jogo, detecção de colisões e cálculo do estado (getState)
├── render.hpp      # Classe Renderer com Raylib
├── render.cpp      # Desenho da janela, grade, cobra, maçã e placar
├── agent.cpp       # Rede neural, memória de replay e lógica de treinamento
├── main.cpp        # Loop principal (jogo manual ou loop de treino do agente)
├── Makefile        # Automação de compilação e execução
└── .gitignore      # Arquivos ignorados pelo Git (binários, objetos, IDE)
```

---

## 🚀 Como Executar

### Pré-requisitos
- Compilador C++ com suporte a **C++17** (`g++`)
- Biblioteca **Raylib** instalada no sistema

### Comandos
```bash
# Compilar e executar o projeto:
make run

# Apenas compilar:
make

# Limpar arquivos compilados:
make clean
```