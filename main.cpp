#include "algo/algo.hpp"
#include "raylib.h"
#include "utility/utility.hpp"
#include <vector>

#define CELL_SIZE 48
#define WINDOW_WIDTH (NUM_COL * CELL_SIZE)
#define WINDOW_HEIGHT (NUM_ROW * CELL_SIZE)


void drawMatrix(MATRIX& maze);

int main() {
    MATRIX maze(NUM_ROW, std::vector<Cell>(NUM_COL, Cell::EMPTY));
    maze[0][0] = Cell::START;
    maze[9][9] = Cell::GOAL;

    
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "searching alg");
    SetTargetFPS(20);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow();


    return 0;
}
/*
plan of attack:
so the point is to use dfs, bfs, and A* to go from start to goal
        for right now, we will not introduce any walls


*/

// 
