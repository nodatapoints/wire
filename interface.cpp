#include "interface.h"


Interface::Interface(sf::RenderWindow &_window, Game &_game):
    window(_window), game(_game), cursor(sf::RectangleShape(cellSize)),
    simulating(false), selecting(false), paused(false), pasting(false) {
    cursor.setOutlineColor(sf::Color(0, 0, 0, 100));
    cursor.setFillColor(sf::Color(0, 0, 0, 0));
    cursor.setOutlineThickness(5);

    clipboard.clear();
    clipboard = {
        {Cell::WIRE, Cell::EMPTY},
        {Cell::EMPTY, Cell::WIRE}
    };
}

void Interface::handleEvent(const sf::Event &event) {
    switch (event.type) {
        case sf::Event::KeyReleased:
            handleKeyReleased(event); break;

        case sf::Event::KeyPressed:
            handleKeyPressed(event); break;

        case sf::Event::MouseButtonReleased:
            handleMouseReleased(event); break;
        
        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseMoved:
            handleMouse(); break;

        default: break;
    }
}

void Interface::handleKeyReleased(const sf::Event &event) {
    switch (event.key.code) {
        case sf::Keyboard::Q:
            window.close();
            break;

        case sf::Keyboard::Escape:
            if (simulating)
                game.recover();

            simulating = false;
            pasting = false;
            paused = false;        

            cursor.setSize(cellSize);
            clipboard.clear();
            break;

        case sf::Keyboard::Space:
            clock.restart();
            if (simulating)
                game.recover();
            else {
                game.backup();
                paused = false;
            }

            simulating = !simulating;
            break;

        case sf::Keyboard::LControl:
            selecting = false;
            if (!pasting)
                cursor.setSize(cellSize);

            break;

        case sf::Keyboard::P:
            paused = !paused;
            if (!simulating) {
                simulating = true;
                game.backup();
            }
            break;

        case sf::Keyboard::S:
            if (!simulating) {
                simulating = paused = true;
                game.backup();
            }
            if (paused)
                game.step();
            break;

        default: break;
    }
}

sf::Vector2i mouseOnGrid() {
    return (sf::Mouse::getPosition()/CELL_SIZE);
}

sf::Vector2f upperLeftCorner(const sf::Vector2i &a, const sf::Vector2i &b) {
    return sf::Vector2f(
        CELL_SIZE*std::min(a.x, b.x),
        CELL_SIZE*std::min(a.y, b.y)
    );
}

sf::Vector2f lowerRightCorner(const sf::Vector2i &a, const sf::Vector2i &b) {
    return sf::Vector2f(
        CELL_SIZE*std::max(a.x, b.x),
        CELL_SIZE*std::max(a.y, b.y)
    );
}


void Interface::setCursor(const sf::Vector2i &pos) {
    cursor.setPosition(static_cast<sf::Vector2f>(pos));
}

void Interface::setCursor(const sf::Vector2f &pos) {
    cursor.setPosition(pos);
}

void Interface::handleKeyPressed(const sf::Event &event) {
    if (event.key.code == sf::Keyboard::LControl) {
        startSelection = mouseOnGrid();
        setCursor(startSelection*CELL_SIZE);
        selecting = true;
    }

    if (selecting && (event.key.code == sf::Keyboard::C || event.key.code == sf::Keyboard::X)) {
        auto pos = sf::Mouse::getPosition() / CELL_SIZE;
        game.copy(pos.x, pos.y, startSelection.x, startSelection.y, clipboard);
        if (event.key.code == sf::Keyboard::X)
            game.setRect(pos.x, pos.y, startSelection.x, startSelection.y, Cell::EMPTY);

        selecting = false;
        pasting = true;
    }

    if (!selecting && (event.key.code == sf::Keyboard::V)) {
        auto pos = sf::Mouse::getPosition() / CELL_SIZE;
        game.paste(pos.x, pos.y, clipboard);
    }
}

void Interface::handleMouseReleased(const sf::Event &event) {
    if (selecting && !simulating) {
        auto pos = sf::Mouse::getPosition() / CELL_SIZE;
        game.setRect(pos.x, pos.y, startSelection.x, startSelection.y, 
            (event.mouseButton.button == sf::Mouse::Left) ? Cell::WIRE : Cell::EMPTY);
         
        if (!pasting)
            cursor.setSize(cellSize);
    }
    selecting = false;
}

void Interface::handleMouse() {
    if (simulating || selecting)
        return;

    auto pos = sf::Mouse::getPosition() / CELL_SIZE;
    cursor.setPosition(pos.x*CELL_SIZE, pos.y*CELL_SIZE);

    Cell cell;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            cell = Cell::ACTIVE;

        else
            cell = Cell::WIRE;
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) 
        cell = Cell::EMPTY;

    else
        return;

    game.setXY(pos.x, pos.y, cell, true);
}

void Interface::draw() {
    if (!simulating) {
        if (selecting) {
            setCursor(upperLeftCorner(startSelection, mouseOnGrid()));
            cursor.setSize(lowerRightCorner(startSelection, mouseOnGrid()) - cursor.getPosition() + cellSize);
        }
        else
            setCursor(mouseOnGrid()*CELL_SIZE);
        window.draw(cursor);

        if (pasting)
            drawClipboard();
    }

    if (simulating && !paused && clock.getElapsedTime().asMilliseconds() > STEP_MS) {
        if (!game.step()) {
            simulating = false;
            game.recover();
        }
        clock.restart();
    }
}

void Interface::drawClipboard() {
    sf::RectangleShape square(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    sf::Color color;
    auto corner = mouseOnGrid();
    for (int x = 0; x < clipboard.size(); ++x) {
        for (int y = 0; y < clipboard.at(0).size(); ++y) {
            square.setPosition((corner.x+x)*CELL_SIZE, (corner.y+y)*CELL_SIZE);
            
            color = colors[static_cast<int>(clipboard.at(x).at(y))];
            color.a = 50;
            square.setFillColor(color);

            window.draw(square);

        }
    }
}