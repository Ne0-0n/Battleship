#include "ship.h"

Ship::Ship(int size, Position coord, Direction dir)
    : _size(size), _coord(coord), _direction(dir) {
    if (size < 1 || size > 4) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    if (is_collision(size, coord, dir)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
}

Ship::Ship(int size, Position coord)
    : _size(size), _coord(coord), _direction(Horizontal) {
    if (size < 1 || size > 4) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    if (is_collision(size, coord, _direction)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
}

Ship::Ship(int size, char direction, int row, char col)
    : _size(size),
    _coord(row, col - 'A' + 1),
    _direction(direction == 'H' ? Horizontal : Vertical) {
    if (size < 1 || size > 4) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    if (direction != 'H' && direction != 'V') {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    if (is_collision(_size, _coord, _direction)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
}

void Ship::rotate() {
    Direction newDir = (_direction == Horizontal) ? Vertical : Horizontal;
    if (is_collision(_size, _coord, newDir)) {
        throw std::logic_error("Invalid input: incorrect ship");
    }
    _direction = newDir;
}

bool Ship::is_collision(int size, const Position& coord, Direction dir) {
    int r = coord.row();
    int c = coord.col();
    if (dir == Horizontal) {
        if (c + size - 1 > 10) return true;
    }
    else {
        if (r + size - 1 > 10) return true;
    }
    return false;
}