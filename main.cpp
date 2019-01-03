#include "wire.h"
#include "interface.h"

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Wire");
    window.setFramerateLimit(20);
    window.setMouseCursorVisible(false);

    Game game(
        window.getSize().x / CELL_SIZE +3,  // mehr puffer
        window.getSize().y / CELL_SIZE +3
    );

    Interface interface(window, game);

    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event))
            interface.handleEvent(event);      
            
        window.clear();
        game.draw(window);
        interface.draw();
        window.display();
    }

    return 0;
}