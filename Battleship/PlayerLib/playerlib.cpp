#include "playerlib.h"
#include <iostream>

const int Player::_max_ships_counts[4] = { 4, 3, 2, 1 };

Player::Player() : _gamefield() {
    for (int i = 0; i < 4; ++i) {
        _ships_counts[i] = 0;
    }
}

void Player::set_ship(const Ship& ship) {
    int sz = ship.size();
    if (sz < 1 || sz > 4) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    if (_ships_counts[sz - 1] >= _max_ships_counts[sz - 1]) {
        throw std::logic_error("Invalid input: incorrect ship");
    }

    int r = ship.row();
    int c = ship.col();
    int dr = (ship.direction() == Vertical) ? 1 : 0;
    int dc = (ship.direction() == Horizontal) ? 1 : 0;

    int end_r = r + dr * (sz - 1);
    int end_c = c + dc * (sz - 1);
    if (end_r > _gamefield.rows() || end_c > _gamefield.cols()) {
        throw std::logic_error("Invalid input: incorrect ship");
    }

    // Проверка коллизий (включая окружающие клетки)
    for (int i = 0; i < sz; ++i) {
        int cr = r + dr * i;
        int cc = c + dc * i;
        for (int x = cr - 1; x <= cr + 1; ++x) {
            for (int y = cc - 1; y <= cc + 1; ++y) {
                if (x < 1 || x > _gamefield.rows()) continue;
                if (y < 1 || y > _gamefield.cols()) continue;
                if (_gamefield.get_raw(x, y) == '*') {
                    throw std::logic_error("Invalid input: incorrect ship");
                }
            }
        }
    }

    // Устанавливаем
    for (int i = 0; i < sz; ++i) {
        int cr = r + dr * i;
        int cc = c + dc * i;
        _gamefield.set_raw(cr, cc, '*');
    }
    _ships_counts[sz - 1]++;
}

State Player::set_action(int row, char col) {
    char cur = _gamefield.get(row, col);
    if (cur == ' ') {
        _gamefield.set(row, col, '.');
        return Missed;
    }
    else if (cur == '*') {
        _gamefield.set(row, col, 'X');
        return determine_hit_state(row, col);
    }
    throw std::logic_error("Invalid input: incorrect move");
}

void Player::show_field(bool hide_ships) {
    std::cout << to_string(_gamefield, hide_ships);
    std::cout << "\nShips Left:\n";
    std::cout << "* - " << (_max_ships_counts[0] - _ships_counts[0]) << " "
        << "** - " << (_max_ships_counts[1] - _ships_counts[1]) << " "
        << "*** - " << (_max_ships_counts[2] - _ships_counts[2]) << " "
        << "**** - " << (_max_ships_counts[3] - _ships_counts[3]) << "\n";
}

bool Player::check_lose() const {
    for (int i = 0; i < 4; ++i) {
        if (_ships_counts[i] > 0) return false;
    }
    return true;
}

bool Player::check_ready() const {
    for (int i = 0; i < 4; ++i) {
        if (_ships_counts[i] != _max_ships_counts[i]) return false;
    }
    return true;
}

GameField& Player::field() {
    return _gamefield;
}

const GameField& Player::field() const {
    return _gamefield;
}

int Player::ships_count(int idx) const {
    return _ships_counts[idx];
}

State Player::determine_hit_state(int row, char col) {
    int n = _gamefield.rows();
    int m = _gamefield.cols();

    bool** visited = new bool* [n + 1];
    for (int i = 0; i <= n; ++i) {
        visited[i] = new bool[m + 1];
        for (int j = 0; j <= m; ++j) visited[i][j] = false;
    }

    int start_r = row;
    int start_c = col - 'A' + 1;

    int* qr = new int[n * m + 1];
    int* qc = new int[n * m + 1];
    int head = 0, tail = 0;
    qr[tail] = start_r;
    qc[tail] = start_c;
    tail++;
    visited[start_r][start_c] = true;

    int count = 0;
    bool still_alive = false;

    int dr4[] = { -1, 1, 0, 0 };
    int dc4[] = { 0, 0, -1, 1 };

    while (head < tail) {
        int cr = qr[head], cc = qc[head];
        head++;
        count++;

        for (int k = 0; k < 4; ++k) {
            int nr = cr + dr4[k];
            int nc = cc + dc4[k];
            if (nr < 1 || nr > n || nc < 1 || nc > m) continue;
            char ch = _gamefield.get_raw(nr, nc);
            if (ch == 'X' && !visited[nr][nc]) {
                visited[nr][nc] = true;
                qr[tail] = nr;
                qc[tail] = nc;
                tail++;
            }
            else if (ch == '*') {
                still_alive = true;
            }
        }
    }

    delete[] qr;
    delete[] qc;
    for (int i = 0; i <= n; ++i) delete[] visited[i];
    delete[] visited;

    if (still_alive) return Hit;

    int size = count;
    _ships_counts[size - 1]--;

    switch (size) {
    case 1: return BoatDestroyed;
    case 2: return DestroyersDestroyed;
    case 3: return CruisersDestroyed;
    case 4: return BattleshipDestroyed;
    default: return Hit;
    }
}