#pragma once
#include <string>
#include "ship.h"

enum State {
    Missed,
    BoatDestroyed,
    DestroyersDestroyed,
    CruisersDestroyed,
    BattleshipDestroyed,
    Hit
};

class GameField {
    char** _field;
    const int _n;
    const int _m;

    void alloc();
    void check(int, char) const;

public:
    GameField();
    GameField(int, int);
    GameField(int);
    GameField(const GameField&);
    ~GameField();

    void set(const Ship&);
    State set(int, char);

    int rows() const;
    int cols() const;

    friend std::string to_string(const GameField&, bool);
};

std::string to_string(const GameField&, bool hide_ships = false);