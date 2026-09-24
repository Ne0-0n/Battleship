#include "gamelib.h"
#include <iostream>
#include <sstream>

Game::Game() : _user(), _computer() {}

void Game::parse_field(Player& p, const std::string& input) {
    std::istringstream iss(input);
    std::string line;
    while (std::getline(iss, line)) {
        if (line.empty()) continue;
        std::istringstream ls(line);
        int size, row;
        char direction, col;
        if (!(ls >> size >> direction >> row >> col)) continue;
        Ship ship(size, direction, row, col);
        p.set_ship(ship);
    }
    if (!p.check_ready()) {
        throw std::logic_error("Invalid input: incorrect field");
    }
}

void Game::user_init(const std::string& input) {
    parse_field(_user, input);
}

void Game::computer_init(const std::string& input) {
    parse_field(_computer, input);
}

State Game::user_move(const std::string& input) {
    std::istringstream iss(input);
    char col;
    int row;
    if (!(iss >> row >> col)) {
        throw std::logic_error("Invalid input: incorrect move");
    }
    return _computer.set_action(row, col);
}

State Game::computer_move() {
    int n = _user.field().rows();
    int m = _user.field().cols();

    // 1. Левая диагональ
    for (int i = 1; i <= n && i <= m; ++i) {
        char ch = _user.field().get_raw(i, i);
        if (ch == ' ' || ch == '*') {
            return _user.set_action(i, char('A' + i - 1));
        }
    }

    // 2. Правая (побочная) диагональ
    for (int i = 1; i <= n && i <= m; ++i) {
        int r = i;
        int c = m - i + 1;
        char ch = _user.field().get_raw(r, c);
        if (ch == ' ' || ch == '*') {
            return _user.set_action(r, char('A' + c - 1));
        }
    }

    // 3. Построчно
    for (int r = 1; r <= n; ++r) {
        for (int c = 1; c <= m; ++c) {
            char ch = _user.field().get_raw(r, c);
            if (ch == ' ' || ch == '*') {
                return _user.set_action(r, char('A' + c - 1));
            }
        }
    }

    throw std::logic_error("Invalid input: incorrect move");
}

bool Game::is_end() {
    return _user.check_lose() || _computer.check_lose();
}

void Game::show_game_window() {
    std::cout << "= COMPUTER GAME FIELD =\n\n";
    _computer.show_field(true);
    std::cout << "\n=== YOUR PLAY FIELD ===\n\n";
    _user.show_field(false);
}

void Game::start() {
    std::string user_input, computer_input, line;

    // Блок пользователя
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        user_input += line + "\n";
    }

    // Блок компьютера
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        computer_input += line + "\n";
    }

    user_init(user_input);
    computer_init(computer_input);

    show_game_window();

    while (!is_end()) {
        // Ход пользователя
        bool user_hit = true;
        while (user_hit && !is_end()) {
            std::string move_line;
            if (!std::getline(std::cin, move_line)) break;
            if (move_line.empty()) continue;
            State st = user_move(move_line);
            user_hit = (st != Missed);
        }

        if (is_end()) break;

        // Ход компьютера
        bool comp_hit = true;
        while (comp_hit && !is_end()) {
            State st = computer_move();
            comp_hit = (st != Missed);
        }
    }

    show_game_window();

    if (_computer.check_lose()) {
        std::cout << "\nUSER WIN!\n";
    }
    else {
        std::cout << "\nCOMPUTER WIN!\n";
    }
}