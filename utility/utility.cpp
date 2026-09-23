#include "utility.hpp"

#include <iostream>
#include <raylib.h>

bool isValid(int row, int col, int matrix_row, int matrix_col) {
    if (row < 0 || row >= matrix_row || col < 0 || col >= matrix_col) return false;
    return true;
}

void printPath(std::vector<Coord> path, std::string type) {
    std::cout << "alg: " << type << '\n';
    for (int i = static_cast<int>(path.size()) - 1; i >= 0; i--) {
        std::cout << "row: " << path[i].first << " col: " << path[i].second << '\n';
    }
}

std::vector<Coord> createPath(std::vector<std::vector<Coord>> parent, int gr, int gc, MATRIX& maze) {
    Coord curr = {gr, gc};
    std::vector<Coord> answer;
    while (curr.first != -1) {
        answer.push_back(curr);
        maze[curr.first][curr.second] = Cell::PATH;
        curr = parent[curr.first][curr.second];
    }

    return answer;
}

Color cellColor(MATRIX &maze, Coord coord) {
    Cell currCell = maze[coord.first][coord.second];
    switch (currCell) {
        case Cell::EMPTY:
            return WHITE;
        case Cell::GOAL:
            return GOLD;
        case Cell::VISITED:
            return GRAY;
        case Cell::START:
            return RED;
        case Cell::PATH:
            return GREEN;
        default:
            return BLACK;
    }
}
