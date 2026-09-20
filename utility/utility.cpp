#include "utility.hpp"

#include <iostream>

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

std::vector<Coord> createPath(std::vector<std::vector<Coord>> parent, int gr, int gc) {
    Coord curr = {gr, gc};
    std::vector<Coord> answer;
    answer.push_back(curr);
    while (curr.first != -1) {
        curr = parent[curr.first][curr.second];
        answer.push_back(curr);
    }

    return answer;
}
