#include "engine.hpp"
#include "config.hpp"
#include <cstdlib>
#include <algorithm>

using namespace std;
Engine::Engine(int w, int h) : width(w), height(h), curr_dir(UP), score(0), game_over(false) {
    reset();
}

void Engine::spawnApple() {
    apple.x = rand() % width;
    apple.y = rand() % height;

    // a maça não pode nascer dentro da cobra
    while (find(snake.begin(), snake.end(), apple) != snake.end()) {
        apple.x = rand() % width;
        apple.y = rand() % height; 
    }

}

bool Engine::checkCollision(Point new_head) {
    // colisão com as paredes
    if (new_head.x < 0 || new_head.x >= width || new_head.y < 0 || new_head.y >= height) {
        return true;
    }

    // colisão com o próprio corpo
    if (find(snake.begin(), snake.end(), new_head) != snake.end()) {
        return true;
    }

    return false;
}

tuple<float, bool> Engine::step(int action) {
    updateDirection(action);

    Point new_head = snake.front();
    if (curr_dir == UP) new_head.y -= 1;
    else if (curr_dir == DOWN) new_head.y += 1;
    else if (curr_dir == LEFT) new_head.x -= 1;
    else if (curr_dir == RIGHT) new_head.x += 1;

    if (checkCollision(new_head)) {
        game_over = true;
        return {Config::PUNISH_LOSE, true};
    }

    snake.push_front(new_head);

    float reward = 0.0f;
    if (new_head == apple) {
        score ++;
        reward = Config::REWARD_EAT;
        spawnApple();
    } else {
        snake.pop_back();
        reward = Config::PUNISH_STEP;
    }
    return {reward, false};
}

vector<float> Engine::getState() {
    vector<float> state(Config::NUM_INPUT, 0.0f);

    // As 3 primeiras entradas
    // Obstáculos à frente
    Point front = snake.front();
    if (curr_dir == UP) front.y -= 1;
    else if (curr_dir == DOWN) front.y += 1;
    else if (curr_dir == LEFT) front.x -= 1;
    else if (curr_dir == RIGHT) front.x += 1;

    state[0] = checkCollision(front) ? 1.0f : 0.0f;

    // Obstáculos à esquerda
    Point left = snake.front();
    if (curr_dir == UP) left.x -= 1;
    else if (curr_dir == DOWN) left.x += 1;
    else if (curr_dir == LEFT) left.y += 1;
    else if (curr_dir == RIGHT) left.y -= 1;

    state[1] = checkCollision(left) ? 1.0f : 0.0f;

    // Obstáculos à direita
    Point right = snake.front();
    if (curr_dir == UP) right.x += 1;
    else if (curr_dir == DOWN) right.x -= 1;
    else if (curr_dir == LEFT) right.y -= 1;
    else if (curr_dir == RIGHT) right.y += 1;

    state[2] = checkCollision(right) ? 1.0f : 0.0f;

    // cima, baixo, esquerda, direita
    if(curr_dir == UP) {
        state[3] = 1.0f; // frente
        state[4] = 0.0f; // trás
        state[5] = 0.0f; // esquerda
        state[6] = 0.0f; // direita
    } else if(curr_dir == DOWN) {
        state[3] = 0.0f;
        state[4] = 1.0f;
        state[5] = 0.0f;
        state[6] = 0.0f;
    } else if(curr_dir == LEFT) {
        state[3] = 0.0f;
        state[4] = 0.0f;
        state[5] = 1.0f;
        state[6] = 0.0f;
    } else if(curr_dir == RIGHT) {
        state[3] = 0.0f;
        state[4] = 0.0f;
        state[5] = 0.0f;
        state[6] = 1.0f;
    }

    // distancia da maça para a cabeça: cima, baixo, esquerda, direita
    if(apple.y < snake.front().y) {
        state[7] = 1.0f; // maça acima
    } else {
        state[7] = 0.0f;
    }

    if(apple.y > snake.front().y) {
        state[8] = 1.0f; // maça abaixo
    } else {
        state[8] = 0.0f;
    }

    if(apple.x < snake.front().x) {
        state[9] = 1.0f; // maça à esquerda
    } else {
        state[9] = 0.0f;
    }

    if(apple.x > snake.front().x) {
        state[10] = 1.0f; // maça à direita
    } else {
        state[10] = 0.0f;
    }

    return state;
}

void Engine::reset() {
    snake.clear();
    snake.push_back({width / 2, height / 2});
    curr_dir = UP;
    score = 0;
    game_over = false;
    spawnApple();
}

void Engine::updateDirection(int action) {
    if (action == ACTION_LEFT) {
        switch (curr_dir) {
            case UP:    curr_dir = LEFT;  break;
            case DOWN:  curr_dir = RIGHT; break;
            case LEFT:  curr_dir = DOWN;  break;
            case RIGHT: curr_dir = UP;    break;
        }
    } else if (action == ACTION_RIGHT) {
        switch (curr_dir) {
            case UP:    curr_dir = RIGHT; break;
            case DOWN:  curr_dir = LEFT;  break;
            case LEFT:  curr_dir = UP;    break;
            case RIGHT: curr_dir = DOWN;  break;
        }
    }
    // Se action == ACTION_UP (ou ACTION_FORWARD), a cobra continua na direção atual
}