#include "algo.hpp"

#include <queue>
#include <set>
#include <stack>

namespace search {

std::pair<bool, std::vector<Coord>> bfs(MATRIX matrix) {
    // parent[r][c] = who discovered (r,c); {-1,-1} means none
    std::vector<std::vector<Coord>> parent(
        NUM_ROW, std::vector<Coord>(NUM_COL, {-1, -1}));

    std::set<Coord> visited;
    std::queue<Coord> q;

    q.push({0, 0});
    visited.insert({0, 0});

    int dir[4][2]{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (auto direction : dir) {
            int nr = r + direction[0];
            int nc = c + direction[1];

            if (isValid(nr, nc) && !visited.count({nr, nc})) {
                parent[nr][nc] = {r, c};
                if (matrix[nr][nc] == Cell::GOAL) {
                    auto path = createPath(parent, nr, nc);
                    return {true, path};
                }
                if (matrix[nr][nc] == Cell::WALL) continue;
                visited.insert({nr, nc});
                q.push({nr, nc});
            }
        }
    }
    return {false, {}};
}

std::pair<bool, std::vector<Coord>> dfs(MATRIX matrix) {
    std::stack<Coord> s;
    std::vector<std::vector<Coord>> parent(NUM_ROW, std::vector<Coord>(NUM_COL, {-1, -1}));
    std::set<Coord> visited;
    int dir[4][2]{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    s.push({0, 0});
    visited.insert({0, 0});
    while (!s.empty()) {
        auto curr = s.top();
        s.pop();
        for (auto direction : dir) {
            Coord child = {curr.first + direction[0], curr.second + direction[1]};
            if (isValid(child.first, child.second) && visited.count(child) < 1) {
                parent[child.first][child.second] = {curr.first, curr.second};
                if (matrix[child.first][child.second] == Cell::GOAL) {
                    auto answer = createPath(parent, child.first, child.second);
                    return {true, answer};
                }
                if (matrix[child.first][child.second] == Cell::WALL) {
                    continue;
                }
                visited.insert(child);
                s.push(child);
            }
        }
    }
    return {false, {}};
}


void bfsInit(searchSpace &ss, Coord start, MATRIX maze) {
    // mark the start as visited
    ss.visited.insert({start});
    // push the start coord onto the queue
    ss.q.emplace(start);
    // set the search state's maze equal to the parameter maze
    ss.maze = maze;
}

std::pair<bool,std::string> bfsStep(searchSpace& ss, Coord goal) {
    // process all four directions at once but only process once child node at the time
    // get the top of the queue
    auto curr = ss.q.front();
    // pop the top of the queue
    ss.q.pop();
    // create the direction arr
    int direction[4][2] = {{-1,0}, {1,0}, {0,1}, {0,-1}};
    // loop through all four directions
    for (auto dir : direction) {
        int nr = curr.first + dir[0];
        int nc = curr.second + dir[1];
        // create the child node from the top node
        Coord child{nr, nc};
        // if the child is valid and has not been visisted yet
        if (isValid(nr, nc) && ss.visited.count(child) < 1) {
            ss.parent[nr][nc] = curr;
            // check if the child node is the goal
            if (ss.maze[nr][nc] == Cell::GOAL) {
                return {true, "goal"};
            }
            // check is the wall
            if (ss.maze[nr][nc] == Cell::WALL) continue;
            // push the child node as visited in the visited set
            ss.visited.insert(child);
            // mark the maze in the child coordinate as visited
            ss.maze[nr][nc] = Cell::VISITED;
            // mark the path in the parent
            // push onto the queue
            ss.q.push(child);
        }
    }
    return {false, "~goal"};
}


}  // namespace search
