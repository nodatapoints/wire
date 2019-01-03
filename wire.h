#ifndef WIRE_H
#define WIRE_H

#include <SFML/Graphics.hpp>

#include <vector>
#include <array>
#include <stdexcept>

// using uint = unsigned int;

enum class Cell : unsigned char {
    EMPTY,
    WIRE,
    BLOCKING,
    ACTIVE,
};

const int CELL_SIZE = 32;

const float STEP_MS = 100;

const sf::Color colors[] = {
    sf::Color::White,
    sf::Color(50, 90, 190),
    sf::Color(33, 60, 128),
    sf::Color(250, 250, 90)
};

using Neighbours = std::array<int, 4>;

using Grid = std::vector< std::vector<Cell> >;

class Game {
public:
    
    int width, height;

    Game(const int &width, const int &height);
    Cell getXY(const int &x, const int &y) const noexcept;
    void setXY(const int &x, const int &y, const Cell &cell, bool front);
    void setRect(int x1, int y1, int x2, int y2, const Cell &cell);
    bool step();
    void backup();
    void recover();
    void copy(int x1, int y1, int x2, int y2, Grid &clipboard) const;
    void paste(const int &x1, const int &y1, const Grid &grid);
    void draw(sf::RenderWindow &window) const;

private:
    Grid front, back, original;

    int countNeighbours(const int &x, const int &y, Cell type) const;
    Cell next(const int &x, const int &y) const;
};


#endif