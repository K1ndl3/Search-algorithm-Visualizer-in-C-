#include "algo/algo.hpp"
#include "raylib.h"
#include <vector>

int main() {
    MATRIX maze(NUM_ROW, std::vector<Cell>(NUM_COL, Cell::EMPTY));
    maze[0][0] = Cell::START;
    maze[9][9] = Cell::GOAL;

    std::pair<bool, std::vector<Coord>> dfsPath = search::dfs(maze);
    std::pair<bool, std::vector<Coord>> bfsPath = search::bfs(maze);
    printPath(dfsPath.second, "dfs");
    printPath(bfsPath.second, "bfs");
    InitWindow(, int height, "searching alg")
    while (!WindowShouldClose()) {
        
    }


    return 0;
}
/*
plan of attack:
so the point is to use dfs, bfs, and A* to go from start to goal
        for right now, we will not introduce any walls


*/

// i need to change the bfs and dfs in such a way that it pops and 
