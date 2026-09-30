#ifndef ALGO_HPP
#define ALGO_HPP

#include "../utility/utility.hpp"

#include <deque>
#include <utility>
#include <vector>
#include <set>
#include <queue>
#include <limits>

namespace search {

    struct Node {
        int eval_score;
        Coord curr_cell;
    };

    // Min-heap ordering: lower eval_score = higher priority
    struct NodeCompare {
        bool operator()(const Node& a, const Node& b) const {
            return a.eval_score > b.eval_score;
        }
    };

    struct searchSpace {
        // BFS: push_back + pop_front; DFS: push_front + pop_front
        std::deque<Coord> q;
        std::set<Coord> visited;
        std::vector<std::vector<Coord>> parent =
        std::vector<std::vector<Coord>>(Config::NUM_ROW, std::vector<Coord>(Config::NUM_COL, {-1, -1}));
        MATRIX maze;
        Coord startCoord;
        Coord endCoord;
        int numStartCell = 1;
        std::priority_queue<Node, std::vector<Node>, NodeCompare> minHeap;
        std::vector<std::vector<int>> gCost = std::vector<std::vector<int>>(Config::NUM_ROW, std::vector<int>(Config::NUM_COL, std::numeric_limits<int>::max()));
    };

    std::pair<int,Coord> getNumStartCell(searchSpace& ss, Cell kind);

    void setMaze(searchSpace& ss, Coord mouseCoord, Cell kind);

    void init(searchSpace& ss, Coord start, MATRIX maze);
    std::pair<bool, std::string> dfsStep(searchSpace& ss);
    std::pair<bool,std::string> bfsStep(searchSpace& ss);
    std::pair<bool, std::string> aStarStep(searchSpace& ss);

    std::pair<bool, std::vector<Coord>> bfs(MATRIX matrix);
    std::pair<bool, std::vector<Coord>> dfs(MATRIX matrix);
}

#endif
