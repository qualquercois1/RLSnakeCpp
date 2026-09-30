#include "engine.hpp"
#include "config.hpp"
#include "render.hpp"
#include "agent.hpp"
#include <algorithm>

using namespace std;

int main() {
    Engine engine(Config::GRID_WIDTH, Config::GRID_HEIGHT);
    Renderer renderer(Config::GRID_WIDTH, Config::GRID_HEIGHT, Config::CELL_SIZE);
    NeuralNetwork agent(Config::NUM_INPUT, Config::NUM_HIDDEN, Config::NUM_OUTPUT);
    engine.reset();

    if (Config::MANUAL_PLAY) {
        int action = ACTION_UP;
        double lastStepTime = 0.0;
        const double stepInterval = 0.15; // Move a cada 150ms

        while (!renderer.closeWindow() && !engine.isGameOver()) {
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                action = ACTION_RIGHT;
            } else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                action = ACTION_LEFT;
            } else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                action = ACTION_UP;
            }
            
            double currentTime = GetTime();
            if (currentTime - lastStepTime >= stepInterval) {
                engine.step(action);
                action = ACTION_UP; // Volta a seguir em frente na nova direção
                lastStepTime = currentTime;
            }

            renderer.render(engine.getSnake(), engine.getApple(), engine.getScore());
        }
        return 0;
    } else {
        while (!engine.isGameOver()) {
            vector<float> state = engine.getState();

            vector<float> nnOutput = agent.forward(state);
            auto max_idx = max_element(nnOutput.begin(), nnOutput.end());

            int action = distance(nnOutput.begin(), max_idx);

            //continua daqui

            
            auto [reward, game_over] = engine.step(ACTION_UP);
        if (game_over) {
            break;
        }
        if (Config::RENDER) {
            renderer.render(engine.getSnake(), engine.getApple(), engine.getScore());
        }
    }
    }
    

    return 0;
}