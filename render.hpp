#ifndef RENDER_HPP
#define RENDER_HPP
#include <raylib.h>
#include <deque>
#include "engine.hpp"

using namespace std;

class Renderer {
private:
    int width;
    int height;
    int cellSize;
public:
    Renderer(int w, int h, int cell_size = 30);
    ~Renderer();

    void render(const deque<Point>& snake, Point apple, int score);
    bool closeWindow();

};

#endif