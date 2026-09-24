#include "position.h"

void Position::row(int row) {
    if (row < 1 || row > _max_row) {
        throw std::logic_error("Invalid input: incorrect position");
    }
    _row = row;
}

void Position::col(int col) {
    if (col < 1 || col > _max_col) {
        throw std::logic_error("Invalid input: incorrect position");
    }
    _col = col;
}

Position::Position() {
    _row = 1;
    _col = 1;
}

Position::Position(int row, int col) {
    if (row < 1 || row > _max_row || col < 1 || col > _max_col) {
        throw std::logic_error("Invalid input: incorrect position");
    }
    _row = row;
    _col = col;
}

Position::Position(const Position& other) {
    _row = other._row;
    _col = other._col;
}

Position parse(const std::string& str) {
    size_t openBracket = str.find('(');
    size_t comma = str.find(',');
    size_t closeBracket = str.find(')');

    if (openBracket == std::string::npos || comma == std::string::npos
        || closeBracket == std::string::npos) {
        throw std::logic_error("Invalid input: incorrect position");
    }

    std::string rowStr = str.substr(openBracket + 1, comma - openBracket - 1);
    std::string colStr = str.substr(comma + 1, closeBracket - comma - 1);

    rowStr.erase(0, rowStr.find_first_not_of(" \t"));
    rowStr.erase(rowStr.find_last_not_of(" \t") + 1);
    colStr.erase(0, colStr.find_first_not_of(" \t"));
    colStr.erase(colStr.find_last_not_of(" \t") + 1);

    int row = std::stoi(rowStr);
    int col = std::stoi(colStr);

    return Position(row, col);
}

Position::Position(const std::string& str) {
    *this = parse(str);
}

std::string to_string(const Position& other) {
    std::string res;
    res += '(';
    res += std::to_string(other._row);
    res += ", ";
    res += std::to_string(other._col);
    res += ')';
    return res;
}