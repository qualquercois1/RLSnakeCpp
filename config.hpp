#ifndef CONFIG
#define CONFIG

#include <cstddef>
using namespace std;

namespace Config {
    constexpr size_t NUM_INPUT = 11; // 11 entradas booleanas
    constexpr size_t NUM_HIDDEN = 256; // 2^8 neuronios na camada oculta
    constexpr size_t NUM_OUTPUT = 3; // apenas 3 saidas float

    constexpr float LEARNING_RATE = 0.001f; // Taxa de aprendizado
    constexpr float GAMMA = 0.90f; // Fator de desconto

    constexpr size_t MAX_MEMORY = 100000;
    constexpr size_t BATCH_SIZE = 1000;

    constexpr int NUM_GAMES_TO_MAP = 80; // Numero de jogos para conhecer o mapa

    constexpr float REWARD_EAT = 10.0f; // recompensa por comer maça
    constexpr float PUNISH_LOSE = -10.0f; // punição por perder

    constexpr float PUNISH_STEP = -0.1f; // punição por cada passo dado

    constexpr size_t GRID_WIDTH = 20; // comprimento da grade do jogo
    constexpr size_t GRID_HEIGHT = 20; // altura da grade do jogo
    constexpr int CELL_SIZE = 30; // tamanho de cada celula da grade

    constexpr bool RENDER = true; // renderizar o jogo ou não
    constexpr bool MANUAL_PLAY = true; // jogar manualmente ou não
}

#endif