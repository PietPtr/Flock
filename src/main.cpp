#include <SFML/Graphics.hpp>
#include "Game.h"
#include <iostream>

/*
Controls:

Mouse movement:     Set goal under mouse pointer
C:                  Goal starts circling
H:                  Hide goal
G:                  Generate square opstacle under mouse
Up:                 Increase birds updated per frame
Down:               Decrease birds updated per frame
1:                  Increase goal circle speed
2:                  Decrease goal circle speed

*/

using namespace sf;
int main()
{
    srand(time(NULL));

    RenderWindow window(VideoMode(1280, 720), "Float");
    window.setVerticalSyncEnabled(true);

    Game game(&window);

    game.initialize();

    while (window.isOpen())
    {
        game.update();
        game.draw();
    }

    return 0;
}
