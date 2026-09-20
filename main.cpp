#include "raylib.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <utility>
#include <set>
#define NUM_ROW 10
#define NUM_COL 10

enum Cell {
    WALL,
    EMPTY,
    START,
    GOAL
};
typedef std::vector<std::vector<Cell>> MATRIX;
typedef std::pair<int, int> Coord;

std::pair<bool, std::vector<Coord>> bfs(MATRIX matrix);
std::pair<bool, std::vector<Coord>> dfs(MATRIX matrix);
bool isValid(int row, int col, int matrix_row = 10, int matrix_col = 10);
std::vector<Coord> createPath(std::vector<std::vector<Coord>> parent, int gr, int gc);

int main() {
    
    MATRIX maze(NUM_ROW, 
            std::vector<Cell>(NUM_COL, Cell::EMPTY));
    maze[0][0] = Cell::START;
    maze[9][9] = Cell::GOAL;

    maze[3][0] = Cell::WALL;

    std::pair<bool, std::vector<Coord>> ans = dfs(maze);
    for (int i = 0; i < ans.second.size(); i++) {
        std::cout << "r: " << ans.second[i].first << " c: " << ans.second[i].second  << '\n';
    }
    return 0;
}

bool isValid(int row, int col, int matrix_row, int matrix_col) {
    if (row < 0 || row >= matrix_row || col < 0 || col >= matrix_col) return false;
    return true;
}

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
                    auto path = createPath(parent,nr, nc);
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

std::vector<Coord> createPath(std::vector<std::vector<Coord>> parent, int gr, int gc) {
    Coord curr = {9,9};
    std::vector<Coord> answer;
    while (curr.first != -1) {
        curr = parent[curr.first][curr.second];
        answer.push_back(curr);
    }

    return answer;
}


std::pair<bool, std::vector<Coord>> dfs(MATRIX matrix) {
    std::stack<Coord> s;
    std::vector<std::vector<Coord>> parent(NUM_ROW, std::vector<Coord>(NUM_COL, {-1,-1}));
    std::set<Coord> visited;
    int dir[4][2]{{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    s.push({0,0});
    visited.insert({0,0});
    while (!s.empty()) {
        auto curr = s.top();
        s.pop();
        for (auto direction : dir) {
            Coord child = {curr.first + direction[0], curr.second + direction[1]};
            if (isValid(child.first, child.second) && visited.count(child) < 1) {
                if (matrix[child.first][child.second] == Cell::GOAL) {
                    auto answer = createPath(parent, 9, 9);
                    return {true, answer};
                }
                if (matrix[child.first][child.second] == Cell::WALL) {
                    continue;
                }
                visited.insert(child);
                s.push(child);
            }
        }
        // get the top
        // pop
        // for every single direction
        //  child = top + direction
        //  if child is valid
        //      if child is goal -> process goal
        //      if child is wall -> continue
        //      mark child as visited and add to the stack
    }
    return {false, {}};
}


/*
    plan of attack:
        so the point is to use dfs, bfs, dikjstra, and A* to go from start to goal
        for right now, we will not introduce any walls

    BFS:
        - the structure of this search is to expand all of the nodes in level
        - bfs uses a queue where first in will be first out (FIFO)
        - to prevent us from searching in a looped manner I will also implement
          a visted set with pairs (row, col)
        - parent[r][c] stores the previous cell that discovered (r, c)
          (no Node* into the queue — that would dangle after pop)
        - so i will need a queue to manage what nodes gets expanded first
        - there will be a condition where if cell::goal, then return
        - there will be a method to reconstruct the path by walking parent
          from goal back to start

        - bfs will run on matrix, it will return a pair of <vector<Coord>, boolean>


*/
