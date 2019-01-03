#ifndef INTERFACE_H
#define INTERFACE_H

#include "wire.h"

const sf::Vector2f cellSize(CELL_SIZE, CELL_SIZE);
const sf::Vector2i cellSizei(CELL_SIZE, CELL_SIZE);

class Interface {
public:
    sf::RenderWindow &window;
    Game &game;
    Grid clipboard;
    sf::Clock clock;
    sf::Vector2i startSelection;
    sf::RectangleShape cursor;
    bool simulating, selecting, paused, pasting;

    Interface(sf::RenderWindow &window, Game &game);
    void handleKeyReleased(const sf::Event &event);
    void handleKeyPressed(const sf::Event &event);
    void handleMouseReleased(const sf::Event &event);
    void handleMouse();

    void handleEvent(const sf::Event &event);
    void draw();
    void drawClipboard();

private:
    void setCursor(const sf::Vector2i &cursor);
    void setCursor(const sf::Vector2f &cursor);
};

#endif