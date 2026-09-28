#pragma once
#include <vector>
#include <deque>
#include <tuple>

using namespace std;

struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum Action {
    ACTION_UP = 0,       // Continua em frente
    ACTION_LEFT = 1,     // Vira à esquerda relativo à cabeça
    ACTION_RIGHT = 2,    // Vira à direita relativo à cabeça
    ACTION_FORWARD = 0
};

class Engine {
    private:
        int width, height;
        deque<Point> snake;
        Point apple;
        Direction curr_dir;
        int score;
        bool game_over;

        void spawnApple();
        bool checkCollision(Point new_head);

    public:
        Engine(int w, int h);

        void reset();

        vector<float> getState();

        // recompensa, game_over
        tuple<float, bool> step(int action);

        void updateDirection(int action);

        deque<Point> getSnake() const {
            return snake;
        }

        Point getApple() const {
            return apple;
        }

        int getScore() const {
            return score;
        }

        bool isGameOver() const {
            return game_over;
        }

};
