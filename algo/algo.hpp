#ifndef ALGO_HPP
#define ALGO_HPP

#include "../utility/utility.hpp"

#include <deque>
#include <utility>
#include <vector>
#include <set>

namespace search {

    struct searchSpace {
        // BFS: push_back + pop_front; DFS: push_front + pop_front
        std::deque<Coord> q;
        std::set<Coord> visited;
        std::vector<std::vector<Coord>> parent =
        std::vector<std::vector<Coord>>(Config::NUM_ROW, std::vector<Coord>(Config::NUM_COL, {-1, -1}));
        MATRIX maze;
        Coord startCoord;
        Coord endCoord;
    };

    void init(searchSpace& ss, Coord start, MATRIX maze);
    std::pair<bool, std::string> dfsStep(searchSpace& ss);
    std::pair<bool,std::string> bfsStep(searchSpace& ss);

    std::pair<bool, std::vector<Coord>> bfs(MATRIX matrix);
    std::pair<bool, std::vector<Coord>> dfs(MATRIX matrix);
}

#endif
