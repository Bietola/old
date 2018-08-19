#include <ctime>

#include "World.h"
#include "Handler.h"
#include "media.h"

int main()
{
    /*
    sf::RenderWindow window(sf::VideoMode(750,750),"hello sfml");
    tgui::Gui gui(window);
    sf::Event e;
    //widgets
    auto vLayout = std::make_shared<tgui::VerticalLayout>();
    auto button = std::make_shared<tgui::Button>();
    auto editBox = std::make_shared<tgui::EditBox>();
    //make layout
    vLayout->setSize(tgui::bindSize(gui));
    vLayout->add(button);
    vLayout->insertSpace(1, 1);
    vLayout->add(editBox);
    gui.add(vLayout);
    //button properties
    button->setText("hello world");
    button->connect("clicked", [&editBox](){editBox->setText("hello to you!");});
    //edit box properties
    editBox->setText("hello");
    //main
    while(window.isOpen()){
        while(window.pollEvent(e)){
            gui.handleEvent(e);
            if(e.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        gui.draw();
        window.display();
    }*/
    //load asserts
    load_media();
    //create window
    sf::RenderWindow window(sf::VideoMode(1500,750),"Wars");
    //main loop
    sf::Clock gameClock;
    Handler* handler = new MatchHandler(window, new World(100, 100, time(NULL)));
    sf::Event e;
    while(window.isOpen())
    {
        while(window.pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
                window.close();
            handler->handleEvent(e);
        }
        handler->handleLogic(gameClock.getElapsedTime());
        gameClock.restart();
        window.clear();
        handler->handleDrawing();
        window.display();
        Handler::change(handler);
    }
    /*
    load_media();
    AnimationHandler animHandler;
    std::cout << animHandler.mAnimations.empty() << std::endl;
    sf::RenderWindow window(sf::VideoMode(750, 750), "animation test");
    window.setFramerateLimit(30);
    window.setVerticalSyncEnabled(false);
    sf::Clock clock;
    sf::Sprite sprite;
    //idle anim
    AnimationInfo idleAnim;
    idleAnim.frameRects = {
        {0, 0, 64, 64},
        {64, 0, 64, 64}
    };
    idleAnim.isLooping = true;
    idleAnim.speed = 5;
    idleAnim.texture = gTextureProvider.getMediaPtr("characters");
    animHandler.addAnimation(idleAnim);
    //attack anim
    AnimationInfo attAnim;
    attAnim.frameRects = {
        {0, 64, 64, 64},
        {64, 64, 64, 64}
    };
    attAnim.isLooping = false;
    attAnim.speed = 5;
    attAnim.texture = gTextureProvider.getMediaPtr("characters");
    while(window.isOpen()){
        sf::Event e;
        while(window.pollEvent(e)){
            if(e.type == sf::Event::Closed)
                window.close();
            if(e.type == sf::Event::KeyPressed)
                animHandler.addAnimation(attAnim);
        }
        animHandler.update(clock.getElapsedTime());
        clock.restart();
        sprite.setTexture(animHandler.getCurrentAnimation().getTexture());
        sprite.setTextureRect(animHandler.getCurrentAnimation().getCurrentFrame());
        window.clear();
        window.draw(sprite);
        window.display();
    }*/
    return 0;
}
