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

}  // namespace search
