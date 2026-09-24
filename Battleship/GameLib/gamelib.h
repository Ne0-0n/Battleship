#pragma once

#include <string>
#include "playerlib.h"

class Game {
    Player _user;
    Player _computer;

    void parse_field(Player& p, const std::string& input);
    void user_init(const std::string& input);
    void computer_init(const std::string& input);

    State user_move(const std::string& input);
    State computer_move();

    bool is_end();
    void show_game_window();

public:
    Game();
    void start();
};