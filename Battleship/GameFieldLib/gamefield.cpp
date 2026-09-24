#include "gamefield.h"
#include <sstream>
#include <stdexcept>

void GameField::alloc() {
    _field = new char* [_n];
    for (int i = 0; i < _n; ++i) {
        _field[i] = new char[_m];
        for (int j = 0; j < _m; ++j)
            _field[i][j] = ' ';
    }
}

void GameField::check(int row, char col) const {
    int c = col - 'A' + 1;
    if (row < 1 || row > _n || c < 1 || c > _m)
        throw std::logic_error("Invalid input: incorrect position");
}

GameField::GameField() : _n(10), _m(10) { alloc(); }
GameField::GameField(int n, int m) : _n(n), _m(m) { alloc(); }
GameField::GameField(int n) : _n(n), _m(n) { alloc(); }

GameField::GameField(const GameField& o) : _n(o._n), _m(o._m) {
    _field = new char* [_n];
    for (int i = 0; i < _n; ++i) {
        _field[i] = new char[_m];
        for (int j = 0; j < _m; ++j)
            _field[i][j] = o._field[i][j];
    }
}

GameField::~GameField() {
    for (int i = 0; i < _n; ++i) delete[] _field[i];
    delete[] _field;
}

void GameField::set(const Ship& ship) {
    int sz = ship.size();
    int r = ship.row();
    int c = ship.col();
    int dr = (ship.direction() == Vertical) ? 1 : 0;
    int dc = (ship.direction() == Horizontal) ? 1 : 0;

    // Проверка: не выходит за поле и не касается других кораблей
    for (int i = 0; i < sz; ++i) {
        int cr = r + dr * i;
        int cc = c + dc * i;
        if (cr < 1 || cr > _n || cc < 1 || cc > _m)
            throw std::logic_error("Invalid input: incorrect field");
        for (int x = cr - 1; x <= cr + 1; ++x) {
            for (int y = cc - 1; y <= cc + 1; ++y) {
                if (x < 1 || x > _n || y < 1 || y > _m) continue;
                if (_field[x - 1][y - 1] == '*')
                    throw std::logic_error("Invalid input: incorrect field");
            }
        }
    }

    // Установка
    for (int i = 0; i < sz; ++i) {
        int cr = r + dr * i;
        int cc = c + dc * i;
        _field[cr - 1][cc - 1] = '*';
    }
}

State GameField::set(int row, char col) {
    check(row, col);
    char& cell = _field[row - 1][col - 'A'];

    if (cell == ' ') {
        cell = '.';
        return Missed;
    }
    if (cell == '.') {
        throw std::logic_error("Invalid input: incorrect move");
    }
    if (cell == 'X') {
        throw std::logic_error("Invalid input: incorrect move");
    }

    // cell == '*': попали
    cell = 'X';

    // Определяем, убит ли весь корабль (BFS по 'X')
    int n = _n, m = _m;
    bool** vis = new bool* [n];
    for (int i = 0; i < n; ++i) {
        vis[i] = new bool[m];
        for (int j = 0; j < m; ++j) vis[i][j] = false;
    }

    int* qr = new int[n * m];
    int* qc = new int[n * m];
    int head = 0, tail = 0;
    qr[tail] = row; qc[tail] = col - 'A' + 1; tail++;
    vis[row - 1][col - 'A'] = true;

    int count = 0;
    bool alive = false;
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
            char ch = _field[nr - 1][nc - 1];
            if (ch == 'X' && !vis[nr - 1][nc - 1]) {
                vis[nr - 1][nc - 1] = true;
                qr[tail] = nr; qc[tail] = nc; tail++;
            }
            else if (ch == '*') {
                alive = true;
            }
        }
    }

    delete[] qr;
    delete[] qc;
    for (int i = 0; i < n; ++i) delete[] vis[i];
    delete[] vis;

    if (alive) return Hit;

    switch (count) {
    case 1: return BoatDestroyed;
    case 2: return DestroyersDestroyed;
    case 3: return CruisersDestroyed;
    case 4: return BattleshipDestroyed;
    default: return Hit;
    }
}

int GameField::rows() const { return _n; }
int GameField::cols() const { return _m; }

std::string to_string(const GameField& f, bool hide_ships) {
    std::ostringstream os;
    os << "  |";
    for (int j = 0; j < f._m; ++j) {
        os << char('A' + j);
        if (j != f._m - 1) os << ' ';
    }
    os << "|\n  +";
    for (int j = 0; j < f._m; ++j) os << "-";
    os << "+\n";
    for (int i = 0; i < f._n; ++i) {
        os << (i + 1);
        if (i + 1 < 10) os << ' ';
        os << "|";
        for (int j = 0; j < f._m; ++j) {
            char c = f._field[i][j];
            if (hide_ships && c == '*') c = ' ';
            os << c;
            if (j != f._m - 1) os << ' ';
        }
        os << "|\n";
    }
    os << "  +";
    for (int j = 0; j < f._m; ++j) os << "-";
    os << "+\n";
    return os.str();
}