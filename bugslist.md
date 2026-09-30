# Bug List — DFS, BFS, and A\* Algorithms

---

## 🔴 BFS (Batch) — `bfs()` in `algo.cpp`

### Bug 1 — Hardcoded start position `{0, 0}` (line 18)
```cpp
q.push({0, 0});
visited.insert({0, 0});
```
BFS always starts from cell (0,0), completely ignoring wherever `Cell::START` is placed in
the matrix. The function should scan the matrix to find the `START` cell first.

---

### Bug 2 — `parent` assigned before wall check (lines 31–37)
```cpp
parent[nr][nc] = {r, c};                         // set unconditionally
if (matrix[nr][nc] == Cell::GOAL) { ... }
if (matrix[nr][nc] == Cell::WALL) continue;      // wall check is too late
```
The parent is assigned to wall cells before confirming they are walkable. Because wall cells
are never inserted into `visited`, they can be re-encountered on subsequent steps, each time
overwriting their parent with incorrect data.

---

## 🔴 DFS (Batch) — `dfs()` in `algo.cpp`

### Bug 3 — Hardcoded start position `{0, 0}` (line 51)
```cpp
s.push({0, 0});
```
Same as Bug 1. DFS always starts at (0,0), ignoring the actual `Cell::START` position.

---

### Bug 4 — `parent` assigned before wall check (lines 58–65)
Same as Bug 2. The parent of a wall cell is set before the wall check fires, polluting the
parent table with entries that should never exist.

---

## 🔴 All Step Variants — `bfsStep`, `dfsStep`, `aStarStep`

### Bug 5 — `parent` array never reset in `setMaze()`
When the user moves the start cell, `setMaze` clears `visited`, `q`, and reseeds the heap,
but **never resets the `parent` array** back to `{-1, -1}`. After a first run, if the start
is moved and the algorithm runs again, `createPath` will follow stale parent pointers for the
new start cell (whose parent entry may have been set during the previous run), producing an
incorrect or garbage path.

**Example:** if `parent[2][0]` was set to `{1, 0}` during Run 1, and (2,0) becomes the new
start in Run 2, `createPath` will walk off into the old path instead of terminating at the
start.

---

### Bug 6 — Return value of `createPath` is discarded
In `bfsStep` (~line 107), `dfsStep` (~line 143), and `aStarStep` (~line 178):
```cpp
createPath(ss.parent, nr, nc, ss.maze);   // return value thrown away
return {true, "goal"};
```
`createPath` returns the path as `std::vector<Coord>`, but it is silently discarded in all
three step variants. Only the maze visualisation is updated via the `maze` reference. Any
caller that needs explicit path coordinates will not receive them.

---

## 🔴 A\* Step — `aStarStep()` in `algo.cpp`

### Bug 7 — `!= startCoord` bypass allows start cell to skip the closed-set check (line 168)
```cpp
if (ss.visited.count(curr.curr_cell) > 0 && curr.curr_cell != ss.startCoord) {
    return {false, "step finished"};
}
```
Because BFS/DFS pre-insert `start` into `visited`, A\*'s closed-set check would incorrectly
skip the start cell on its first pop. The `!= startCoord` workaround bypasses the check for
the start cell entirely, meaning the start cell **never** gets filtered by the visited check.
If the start were somehow pushed into the heap a second time the check would still let it
through. The correct fix is to not pre-seed `visited` with `start` for A\*, or to use a
separate closed set.

---

### Bug 8 — `endCoord` defaults to `{0, 0}` before a goal is placed
`searchSpace::endCoord` is default-initialised as `std::pair<int,int>`, giving `{0, 0}`. If
A\* runs (or `setMaze` with `Cell::START` is called) before any goal cell has been placed,
`manhattenDistance` uses `{0, 0}` as the target, producing a completely wrong heuristic for
every cell. A\* will behave incorrectly until a goal is explicitly placed.

---

## 🟡 `getNumStartCell()` in `algo.cpp`

### Bug 9 — Column loop uses row count as its bound (line 255)
```cpp
for (int col = 0; col < ss.maze.size(); col++) {   // BUG: should be ss.maze[row].size()
```
`ss.maze.size()` returns the **row** count (`NUM_ROW`), not the column count. This should be
`ss.maze[row].size()`. Currently harmless because the grid is square (20×20), but will cause
out-of-bounds access or silently wrong results the moment `NUM_ROW != NUM_COL`.

---

## 🟡 `createPath()` in `utility.cpp`

### Bug 10 — `parent` passed by value — unnecessary full copy
```cpp
std::vector<Coord> createPath(std::vector<std::vector<Coord>> parent, ...);
//                                                               ^^^^^^ by value
```
The entire `NUM_ROW × NUM_COL` parent array is copied on every call. It should be passed as
`const std::vector<std::vector<Coord>>&` to avoid the O(rows × cols) allocation and copy
that happens each time a path is reconstructed.

---

## Summary Table

| # | Severity | Location | Description |
|---|----------|----------|-------------|
| 1 | 🔴 Bug | `bfs()` line 18 | Hardcoded start `{0,0}` — ignores `Cell::START` in matrix |
| 2 | 🔴 Bug | `bfs()` lines 31–37 | `parent` set before wall check |
| 3 | 🔴 Bug | `dfs()` line 51 | Hardcoded start `{0,0}` — ignores `Cell::START` in matrix |
| 4 | 🔴 Bug | `dfs()` lines 58–65 | `parent` set before wall check |
| 5 | 🔴 Bug | `setMaze()` | `parent` array never reset → stale paths after moving start |
| 6 | 🔴 Bug | `bfsStep` / `dfsStep` / `aStarStep` | `createPath` return value discarded |
| 7 | 🔴 Bug | `aStarStep()` line 168 | `!= startCoord` hack — start cell bypasses closed-set check |
| 8 | 🔴 Bug | `aStarStep()` | `endCoord` defaults to `{0,0}` — wrong heuristic before goal placed |
| 9 | 🟡 Bug | `getNumStartCell()` line 255 | Column loop bound uses row count — breaks on non-square grids |
| 10 | 🟡 Perf | `createPath()` signature | `parent` passed by value — needless O(N²) copy per call |
