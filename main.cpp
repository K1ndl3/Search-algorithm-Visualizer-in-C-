#include "UI/UI.hpp"
#include "algo/algo.hpp"
#include "raylib.h"
#include "config.hpp"
#include "utility/utility.hpp"
#include <vector>





int main() {
    MATRIX maze(Config::NUM_ROW, std::vector<Cell>(Config::NUM_COL, Cell::EMPTY));
    maze[0][0] = Cell::START;
    maze[9][9] = Cell::GOAL;
    search::searchSpace ss;
    search::init(ss, {0,0},maze);

    UI::run(ss);

    return 0;
}
/*
plan of attack:
so the point is to use dfs, bfs, and A* to go from start to goal
        for right now, we will not introduce any walls


*/

// 
