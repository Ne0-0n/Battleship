#pragma once
#include "position.h"

enum Direction { Horizontal, Vertical };

class Ship {
    int _size;
    Position _coord;
    Direction _direction;

public:
    Ship(int, Position, Direction);
    Ship(int, Position);
    Ship(int, char, int, char);
    Ship() = delete;
    Ship(const Ship&) = default;

    int size() const;
    Direction direction() const;
    int row() const;
    int col() const;

    void rotate();

    static bool is_collision(int, const Position&, Direction);
};