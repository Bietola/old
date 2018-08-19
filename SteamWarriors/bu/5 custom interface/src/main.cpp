#include "World.h"
#include "Handler.h"
#include "media.h"
#include "globals.h"

#include "Interface/Button.h"
#include "Interface/PictureBox.h"
#include "Interface/Bar.h"

#include <TGUI/tgui.hpp>

int main()
{
    //start game
    start_game();
    //main loop
    sf::Clock gameClock;
    sf::Event e;
    while(gWindow.isOpen())
    {
        while(gWindow.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                gWindow.close();
            gGameState->handleEvent(e);
        }
        gGameState->handleLogic(gameClock.getElapsedTime());
        gameClock.restart();
        gWindow.clear();
        gGameState->handleDrawing();
        gWindow.display();
        Handler::change(gGameState);
    }
    return 0;
}
