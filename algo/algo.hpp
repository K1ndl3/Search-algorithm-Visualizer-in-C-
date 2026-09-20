#ifndef ALGO_HPP
#define ALGO_HPP

#include "../utility/utility.hpp"

#include <queue>
#include <utility>
#include <vector>
#include <set>

namespace search {

    struct searchSpace {
        std::queue<Coord> q;
        std::set<Coord> visited;
        std::vector<std::vector<Coord>> parent =
        std::vector<std::vector<Coord>>(NUM_ROW, std::vector<Coord>(NUM_COL, {-1, -1}));
        MATRIX maze;
    };

    void bfsInit(searchSpace& ss, Coord start, MATRIX maze);
    std::pair<bool,std::string> bfsStep(searchSpace& ss, Coord goal);

    std::pair<bool, std::vector<Coord>> bfs(MATRIX matrix);
    std::pair<bool, std::vector<Coord>> dfs(MATRIX matrix);
}

#endif
