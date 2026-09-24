#pragma once
#include "../algo/algo.hpp"
#include "../config.hpp"

namespace UI {

    enum algorithm {
        dfs,
        bfs,
        a_start
    };

    enum State {
        setting,
        building,
        searching
    };
    
    struct UI_SETTING{
        bool isSearching = false;
        algorithm searchAlg;
        State currState = State::setting;
    };
    
    void run(search::searchSpace& ss);

    void drawMaze(search::searchSpace& ss);

    void settingsPage(UI_SETTING& setting);
}


// so the goal is to be able to decide between the different options of searching alg and move 
// from the setting screen to the searching screen
