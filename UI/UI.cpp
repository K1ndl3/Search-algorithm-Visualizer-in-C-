#include "./UI.hpp"
#include <raylib.h>
#include <iostream>

void UI::run(search::searchSpace& ss) {
    SetTargetFPS(Config::FPS);
    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, Config::TITLE);
    bool searching = true;
    while (!WindowShouldClose()) {

        BeginDrawing();
    
        ClearBackground(BLACK);
        drawMaze(ss);
        if (searching) {
            auto res = search::dfsStep(ss);
            if (res.first || res.second == "~goal") {
                if (res.second == "~goal")
                    std::cout << "path not found\n";
                searching = false;
            }
        }

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