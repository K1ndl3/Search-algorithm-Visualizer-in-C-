#include "./UI.hpp"
#include <raylib.h>
#include <iostream>

void UI::run(search::searchSpace& ss) {
    SetTargetFPS(Config::FPS);
    InitWindow(Config::SCREEN_WIDTH + 500, Config::SCREEN_HEIGHT, Config::TITLE);
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

        } else if (uiSetting.currState == State::building) {
            buildingPage(uiSetting, ss);
            // create the grid to draw
            // the flow will be draw grid -> select alg ->search
        }else {
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
    Color aColor = (setting.isSearching && setting.searchAlg == algorithm::bfs) ? YELLOW : WHITE;
    Color bColor = (setting.isSearching && setting.searchAlg == algorithm::dfs) ? YELLOW : WHITE;

    if (IsKeyPressed(KEY_ONE)) {
        setting.searchAlg = algorithm::bfs;
        setting.isSearching = true;
    }
    if (IsKeyPressed(KEY_TWO)) {
        setting.searchAlg = algorithm::dfs;
        setting.isSearching = true;
    }

    DrawRectangle(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, GRAY);
    DrawText("Press 1 and then Enter to run BFS", 1, 1, 30, aColor);
    DrawText("Press 2 and then Enter to run DFS", 1, 35, 30, bColor);

    if (setting.isSearching && IsKeyPressed(KEY_ENTER)) {
        setting.currState = State::searching;
    }
}

void UI::buildingPage(UI_SETTING& setting, search::searchSpace& ss) {
    int pageOffset = 900;
    Color aColor = (setting.bChoice == UI::buildingChoice::start) ? YELLOW : WHITE;
    Color bColor = (setting.bChoice == UI::buildingChoice::goal) ? YELLOW : WHITE;
    Color cColor = (setting.bChoice == UI::buildingChoice::wall) ? YELLOW : WHITE;

    if (IsKeyPressed(KEY_ONE)) setting.bChoice = UI::buildingChoice::start;
    if (IsKeyPressed(KEY_TWO)) setting.bChoice = UI::buildingChoice::goal;
    if (IsKeyPressed(KEY_THREE)) setting.bChoice = UI::buildingChoice::wall;

    DrawRectangle(0, 0, Config::SCREEN_WIDTH + pageOffset, Config::SCREEN_HEIGHT, GRAY);
    DrawText("Press 1 to add start cell", Config::SCREEN_WIDTH, 1, 30, aColor);
    DrawText("Press 2 to add goal cell", Config::SCREEN_WIDTH, 35, 30, bColor);
    DrawText("Press 3 to add start wall", Config::SCREEN_WIDTH, 65, 30, cColor);
    DrawText("Press Enter to chose Algorithm", Config::SCREEN_WIDTH, 95, 30, WHITE);
    
    if (IsKeyPressed(KEY_ENTER)) {
        setting.currState = State::setting;
    }

    // logic that actually set up the maze here
    if (setting.bChoice == UI::buildingChoice::start) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isClickInBounds()) {
            auto mouseCoord = getCell(GetMousePosition());
            search::setMaze(ss, mouseCoord, Cell::START);
        }
    }

    drawMaze(ss);
}
