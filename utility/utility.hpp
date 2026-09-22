#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <utility>
#include <vector>
#include "../config.hpp"
#include "raylib.h"

enum Cell {
    WALL,
    VISITED,
    EMPTY,
    START,
    GOAL
};

typedef std::vector<std::vector<Cell>> MATRIX;
typedef std::pair<int, int> Coord;

bool isValid(int row, int col, int matrix_row = Config::NUM_ROW, int matrix_col = Config::NUM_COL);
void printPath(std::vector<Coord> path, std::string type);
std::vector<Coord> createPath(std::vector<std::vector<Coord>> parent, int gr, int gc);
Color cellColor(MATRIX& maze, Coord coord);

#endif
