#include "./UI.hpp"
#include <raylib.h>

void UI::run(search::searchSpace& ss) {
    SetTargetFPS(Config::FPS);
    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, Config::TITLE);

    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(BLACK);
        drawMaze(ss);
        search::bfsStep(ss);

        EndDrawing();
    }

    CloseWindow();
}

void UI::drawMaze(search::searchSpace &ss) {
    for (int row = 0; row < Config::NUM_ROW; row++) {
        for (int col = 0; col < Config::NUM_COL; col++) {
            int posX = Config::CELL_SIZE * col;
            int posY = Config::CELL_SIZE * row;
            Color color = cellColor(ss.maze, {row,col});
            DrawRectangle(posX, posY, Config::CELL_SIZE - 1, Config::CELL_SIZE - 1, color);
        }
    }
}