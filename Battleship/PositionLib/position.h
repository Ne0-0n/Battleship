#pragma once

#include <string>
#include <stdexcept>

class Position {
    int _row;
    int _col;
    static const int _max_row = 10;
    static const int _max_col = 10;

public:
    inline int row() const noexcept {
        return _row;
    }

    inline int col() const noexcept {
        return _col;
    }

    void row(int);
    void col(int);

    Position();
    Position(int, int);
    Position(const Position& other);
    Position(const std::string& str);

    friend std::string to_string(const Position& pos);
    friend Position parse(const std::string& str);
};

