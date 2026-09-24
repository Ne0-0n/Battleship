#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include "ship.h"
#include "gamefieldlib.h"

class Player {
    GameField _gamefield;
    int _ships_counts[4];

    static const int _max_ships_counts[4];

public:
    Player() noexcept;

    void set_ship(const Ship& ship);

    State set_action(int row, char col);
    void show_field(bool hide_ships = false) const;

    bool check_lose() const noexcept;
    bool check_ready() const noexcept;
};

const int Player::_max_ships_counts[4] = { 4, 3, 2, 1 };

inline Player::Player() noexcept : _gamefield() {

    for (int i = 0; i < 4; ++i) {
        _ships_counts[i] = 0;
    }
}

inline void Player::set_ship(const Ship& ship) {
    int size = ship.size();

    if (size < 1 || size > 4) {
        throw std::logic_error("Invalid input: incorrect field");
    }

    if (_ships_counts[size - 1] >= _max_ships_counts[size - 1]) {
        throw std::logic_error("Invalid input: incorrect field");
    }

    _gamefield.set(ship);
    _ships_counts[size - 1]++;
}

inline State Player::set_action(int row, char col) {
    State result = _gamefield.set(row, col);

    if (result == State::BoatDestroyed) _ships_counts[0]--;
    else if (result == State::DestroyersDestroyed) _ships_counts[1]--;
    else if (result == State::CruisersDestroyed) _ships_counts[2]--;
    else if (result == State::BattleshipDestroyed) _ships_counts[3]--;

    return result;
}

inline void Player::show_field(bool hide_ships) const {
    std::cout << to_string(_gamefield, hide_ships) << std::endl;

    std::cout << "Ships Left:" << std::endl;
    std::cout << "* - " << _ships_counts[0] << " "
        << "** - " << _ships_counts[1] << " "
        << "*** - " << _ships_counts[2] << " "
        << "**** - " << _ships_counts[3] << std::endl;
}

inline bool Player::check_lose() const noexcept {
    return (_ships_counts[0] == 0 &&
        _ships_counts[1] == 0 &&
        _ships_counts[2] == 0 &&
        _ships_counts[3] == 0);
}

inline bool Player::check_ready() const noexcept {
    return (_ships_counts[0] == _max_ships_counts[0] &&
        _ships_counts[1] == _max_ships_counts[1] &&
        _ships_counts[2] == _max_ships_counts[2] &&
        _ships_counts[3] == _max_ships_counts[3]);
}