#pragma once

namespace Config {
    constexpr int NUM_COL = 20;
    constexpr int NUM_ROW = 20;
    inline int CELL_SIZE = 50;
    inline int SCREEN_WIDTH = NUM_COL * CELL_SIZE;
    inline int SCREEN_HEIGHT = NUM_ROW * CELL_SIZE;
    inline const char* TITLE = "Searching algorithm visualizer";
    inline int FPS = 20;
}
