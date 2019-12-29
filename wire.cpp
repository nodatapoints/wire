#include "wire.h"

Game::Game(const int &_width, const int &_height): width(_width), height(_height) {
    front = Grid(width, std::vector<Cell>(height, Cell::EMPTY));
    back = Grid(width, std::vector<Cell>(height, Cell::EMPTY));
}

void Game::backup() {
    original = front;
}

void Game::recover() {
    front = original;
}

void Game::copy(int x1, int y1, int x2, int y2, Grid &clipboard) const {
    if (x2 < x1) std::swap(x1, x2);
    if (y2 < y1) std::swap(y1, y2);

    clipboard.resize(x2-x1+1);
    for (int x = x1; x <= x2; ++x) {
        auto &column = clipboard.at(x-x1);
        column.resize(y2-y1+1);
        for (int y = y1; y <= y2; ++y) {
            column.at(y-y1) = getXY(x, y);
        }
    }
}

void Game::paste(const int &x, const int &y, const Grid &grid) {
    for (int dx = 0; dx < grid.size(); ++dx)
        for (int dy = 0; dy < grid.at(0).size(); ++dy)
            if (grid.at(dx).at(dy) != Cell::EMPTY)
                setXY(x+dx, y+dy, grid.at(dx).at(dy), true);
}

Cell Game::getXY(const int &x, const int &y) const noexcept {
    try {
        return front.at(x).at(y);
    }
    catch (const std::out_of_range &err) {
        return Cell::EMPTY;
    }
}

void Game::setXY(const int &x, const int &y, const Cell &cell, bool front=false) {
    if (front)
        this->front.at(x).at(y) = cell;

    else back.at(x).at(y) = cell;
}

void Game::setRect(int x1, int y1, int x2, int y2, const Cell &cell) {
    if (x2 < x1) std::swap(x1, x2);
    if (y2 < y1) std::swap(y1, y2);

    for (int x = x1; x <= x2; ++x)
        for (int y = y1; y <= y2; ++y)
            this->setXY(x, y, cell, true);
}

bool Game::step() {
    Cell cell;
    bool activePart = false;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            // Case for perfect forwarding??
            cell = this->next(x, y);
            this->setXY(x, y, cell);
            if (cell == Cell::ACTIVE)
                activePart = true;
        }
    }
    std::swap(front, back);
    return activePart;
}

int Game::countNeighbours(const int &x, const int &y, Cell type) const {
    int count = 0;
    // I want to use maps, but eeeh
    if (this->getXY(x, y+1) == type) ++count;
    if (this->getXY(x+1, y) == type) ++count;
    if (this->getXY(x, y-1) == type) ++count;
    if (this->getXY(x-1, y) == type) ++count;
#if defined(EXTEND_NEIGHBOURHOOD) || defined(WIREWORLD)
    if (this->getXY(x-1, y-1) == type) ++count;
    if (this->getXY(x-1, y+1) == type) ++count;
    if (this->getXY(x+1, y-1) == type) ++count;
    if (this->getXY(x+1, y+1) == type) ++count;
#endif
    return count;
}

Cell Game::next(const int &x, const int &y) const {
    auto active = this->countNeighbours(x, y, Cell::ACTIVE);

    switch (this->getXY(x, y)) {
        case Cell::ACTIVE:
            return Cell::BLOCKING;

        case Cell::BLOCKING:
            return Cell::WIRE;

        case Cell::WIRE:
#ifdef WIREWORLD
            return (active == 1 || active == 2) ? Cell::ACTIVE : Cell::WIRE;
#else
            return (active % 2 == 0) ? Cell::WIRE : Cell::ACTIVE;
#endif
        case Cell::EMPTY:
        default:
            return Cell::EMPTY;
    }
}

void Game::draw(sf::RenderWindow &window) const {
    sf::RectangleShape square(sf::Vector2f(CELL_SIZE, CELL_SIZE));

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            square.setPosition(x*CELL_SIZE, y*CELL_SIZE);
            square.setFillColor(
                colors[static_cast<int>(getXY(x, y))]
            );
            window.draw(square);
        }
    }
}
