#include "render.hpp"
#include "config.hpp"

Renderer::Renderer(int w, int h, int cell_size) : width(w), height(h), cellSize(cell_size) {
    InitWindow(width * cellSize, height * cellSize, "Snake Game");
    SetTargetFPS(60);
}

Renderer::~Renderer() {
    CloseWindow();
}

bool Renderer::closeWindow() {
    return WindowShouldClose();
}

void Renderer::render(const deque<Point>& snake, Point apple, int score) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i=0; i<width; i++) {
        DrawLine(i * cellSize, 0, i * cellSize, height * cellSize, LIGHTGRAY);
        DrawLine(0, i * cellSize, width * cellSize, i * cellSize, LIGHTGRAY);
    }

    DrawRectangle(apple.x * cellSize, apple.y * cellSize, cellSize, cellSize, RED);

    for (size_t i=0; i<snake.size(); i++) {
        Color cor = (i == 0) ? DARKGREEN : GREEN;
        DrawRectangle(snake[i].x * cellSize, snake[i].y * cellSize, cellSize, cellSize, cor);
    }

    DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);

    EndDrawing();
}   