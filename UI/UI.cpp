#include "./UI.hpp"
#include <raylib.h>
#include <iostream>

void UI::run(search::searchSpace& ss) {
    SetTargetFPS(Config::FPS);
    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, Config::TITLE);
    bool searching = true;
    UI_SETTING uiSetting{};
    while (!WindowShouldClose()) {

        BeginDrawing();

        // create a switch case:
            // setting state
                // draw the setting screen
                // building state
                // set the goal, start, and walls
                // searching state
                // searching alg will run
                
        ClearBackground(BLACK);
        if (uiSetting.currState == State::setting) {
            settingsPage(uiSetting);

        } else {
            drawMaze(ss);
            if (searching) {
                std::pair<bool, std::string> res;
                if (uiSetting.searchAlg == algorithm::bfs) {
                    res = search::bfsStep(ss);
                } else if (uiSetting.searchAlg == algorithm::dfs) {
                    res = search::dfsStep(ss);
                } else {
                    // do nothing for now
                    continue;
                }
                if (res.first || res.second == "~goal") {
                    if (res.second == "~goal")
                        std::cout << "path not found\n";
                    searching = false;
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
}

void UI::drawMaze(search::searchSpace &ss) {
    for (int row = 0; row < Config::NUM_ROW; row++) {
        for (int col = 0; col < Config::NUM_COL; col++) {
            int posX = Config::CELL_SIZE * col;
            int posY = Config::CELL_SIZE * row;
            Color color = cellColor(ss.maze, {row,col});
            DrawRectangle(posX, posY, Config::CELL_SIZE - 1, Config::CELL_SIZE - 1, color);
        }
    }
}

void UI::settingsPage(UI::UI_SETTING& setting) {
    DrawRectangle(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, GRAY);
    // draw the options on the screen
    if (IsKeyPressed(KEY_A)) {
        setting.searchAlg = algorithm::bfs;
        setting.isSearching = true;
    DrawRectangle(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, RED);

    }
    if (IsKeyPressed(KEY_B)){
        setting.searchAlg = algorithm::dfs;
        setting.isSearching = true;
    DrawRectangle(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, RED);

    } 
    // show that enter is now 
    if (setting.isSearching) { 
        if (IsKeyPressed(KEY_ENTER)) {
            setting.currState = State::searching;
        }
    }
}