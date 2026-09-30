#include "algo.hpp"

#include <algorithm>
#include <queue>
#include <set>
#include <stack>

namespace search {

std::pair<bool, std::vector<Coord>> bfs(MATRIX matrix) {
    // parent[r][c] = who discovered (r,c); {-1,-1} means none
    std::vector<std::vector<Coord>> parent(
        Config::NUM_ROW, std::vector<Coord>(Config::NUM_COL, {-1, -1}));

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
                    auto path = createPath(parent, nr, nc, matrix);
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
    std::vector<std::vector<Coord>> parent(Config::NUM_ROW, std::vector<Coord>(Config::NUM_COL, {-1, -1}));
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
                    auto answer = createPath(parent, child.first, child.second, matrix);
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


void init(searchSpace &ss, Coord start, MATRIX maze) {
    ss.maze = maze;
    ss.startCoord = start;
    // Seed the frontier only when the maze already has a START cell.
    // Otherwise building/setMaze will seed once the user places start.
    if (isValid(start.first, start.second) && maze[start.first][start.second] == Cell::START) {
        ss.visited.insert(start);
        ss.q.push_back(start);
    }
}

std::pair<bool,std::string> bfsStep(searchSpace& ss) {
    if (ss.q.empty()) return {false, "~goal"};
    // process all four directions at once but only process once child node at the time
    // get the front of the deque
    auto curr = ss.q.front();
    // pop the front of the deque
    ss.q.pop_front();
    // create the direction arr
    int direction[4][2] = {{-1,0}, {1,0}, {0,1}, {0,-1}};
    // loop through all four directions
    for (auto dir : direction) {
        int nr = curr.first + dir[0];
        int nc = curr.second + dir[1];
        // create the child node from the front node
        Coord child{nr, nc};
        // if the child is valid and has not been visisted yet
        if (isValid(nr, nc) && ss.visited.count(child) < 1) {
            ss.parent[nr][nc] = curr;
            // check if the child node is the goal
            if (ss.maze[nr][nc] == Cell::GOAL) {
                createPath(ss.parent, nr, nc, ss.maze);
                return {true, "goal"};
            }
            // check is the wall
            if (ss.maze[nr][nc] == Cell::WALL) continue;
            // push the child node as visited in the visited set
            ss.visited.insert(child);
            // mark the maze in the child coordinate as visited
            ss.maze[nr][nc] = Cell::VISITED;
            // mark the path in the parent
            // push onto the back (BFS)
            ss.q.push_back(child);
        }
    }
    return {false, "step finished"};
}


std::pair<bool, std::string> dfsStep(searchSpace& ss) {
    // if the deque is empty, goal was not reached
    if (ss.q.empty()) return {false, "~goal"};
    // pop_front the top
    Coord curr = ss.q.front();
    ss.q.pop_front();
    // set up directional array
    int direction[4][2] = {{-1,0}, {1,0}, {0,1}, {0,-1}};
    // for every direction
    for (auto dir : direction) {
    // create a child node from the direction
        Coord child({curr.first + dir[0], curr.second + dir[1]});
        // if child is valid
        if (isValid(child.first, child.second) && ss.visited.count(child) < 1) {
            // mark the parent
            ss.parent[child.first][child.second] = curr;
            // check if goal
            if (ss.maze[child.first][child.second] == Cell::GOAL) {
                createPath(ss.parent, child.first, child.second, ss.maze);
                return {true, "goal"};
            }
            // check if wall
            if (ss.maze[child.first][child.second] == Cell::WALL) continue;
            // else push onto visited set
            ss.visited.insert({child});
            // change the maze
            ss.maze[child.first][child.second] = Cell::VISITED;
            // push_front onto the queue
            ss.q.push_front(child);
        }
    }
    return {false, "step finished"};
}

std::pair<bool, std::string> aStarStep(searchSpace& ss) {
    if (ss.minHeap.empty()) return {false, "~goal"};

    Node curr = ss.minHeap.top();
    ss.minHeap.pop();
    int r = curr.curr_cell.first;
    int c = curr.curr_cell.second;

    // Start is pre-inserted into visited for BFS/DFS; allow it through once for A*.
    if (ss.visited.count(curr.curr_cell) > 0 && curr.curr_cell != ss.startCoord) {
        return {false, "step finished"};
    }

    ss.visited.insert(curr.curr_cell);
    if (ss.maze[r][c] != Cell::GOAL && ss.maze[r][c] != Cell::START) {
        ss.maze[r][c] = Cell::VISITED;
    }

    if (ss.maze[r][c] == Cell::GOAL) {
        createPath(ss.parent, r, c, ss.maze);
        return {true, "goal"};
    }

    int direction[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (auto dir : direction) {
        int nr = r + dir[0];
        int nc = c + dir[1];
        if (!isValid(nr, nc) || ss.visited.count({nr, nc}) > 0) continue;
        if (ss.maze[nr][nc] == Cell::WALL) continue;

        int newG = ss.gCost[r][c] + 1;
        if (newG >= ss.gCost[nr][nc]) continue;

        ss.gCost[nr][nc] = newG;
        ss.parent[nr][nc] = {r, c};
        int h = manhattenDistance({nr, nc}, ss.endCoord);
        int f = newG + h;
        ss.minHeap.push({f, {nr, nc}});
    }
    return {false, "step finished"};
}



void setMaze(searchSpace& ss, Coord mouseCoord, Cell kind) {
    if (kind == Cell::START) {
        auto startCellStatus = getNumStartCell(ss, Cell::START);
        if (startCellStatus.first >= 1) {
            ss.maze[startCellStatus.second.first][startCellStatus.second.second] = Cell::EMPTY;
        }
        ss.maze[mouseCoord.first][mouseCoord.second] = Cell::START;
        ss.startCoord = mouseCoord;
        ss.visited.clear();
        ss.visited.insert(mouseCoord);
        ss.q.clear();
        ss.q.push_back(mouseCoord);

        // Seed A* frontier
        ss.minHeap = {};
        for (auto& row : ss.gCost) {
            std::fill(row.begin(), row.end(), std::numeric_limits<int>::max());
        }
        ss.gCost[mouseCoord.first][mouseCoord.second] = 0;
        int h = manhattenDistance(mouseCoord, ss.endCoord);
        ss.minHeap.push({h, mouseCoord});
        return;
    }
    if (kind == Cell::GOAL) {
        auto startCellStatus = getNumStartCell(ss, Cell::GOAL);
        if (startCellStatus.first >= 1) {
            ss.maze[startCellStatus.second.first][startCellStatus.second.second] = Cell::EMPTY;
        }
        ss.maze[mouseCoord.first][mouseCoord.second] = Cell::GOAL;
        ss.endCoord = mouseCoord;

        // Refresh A* start entry with updated heuristic
        if (isValid(ss.startCoord.first, ss.startCoord.second) &&
            ss.maze[ss.startCoord.first][ss.startCoord.second] == Cell::START) {
            ss.minHeap = {};
            for (auto& row : ss.gCost) {
                std::fill(row.begin(), row.end(), std::numeric_limits<int>::max());
            }
            ss.gCost[ss.startCoord.first][ss.startCoord.second] = 0;
            int h = manhattenDistance(ss.startCoord, ss.endCoord);
            ss.minHeap.push({h, ss.startCoord});
        }
        return;
    }
    ss.maze[mouseCoord.first][mouseCoord.second] = kind;
}

std::pair<int, Coord> getNumStartCell(searchSpace& ss, Cell kind) {
    // need a function to switch the start cell if there is more than one start
    Coord cell;
    int numCells = 0;
    for (int row = 0; row < ss.maze.size(); row++) {
        for (int col = 0; col < ss.maze.size(); col++) {
            if (ss.maze[row][col] == kind) {
                cell = {row, col};
                numCells++;
            }
            if (numCells == 2) cell = {row, col};
        }
    }
    return {numCells, cell};
}

}  // namespace search
