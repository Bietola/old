#include "DialogHandler.h"

///DialogHandler functions
//constructor
DialogHandler::DialogHandler(sf::RenderWindow& window, World& world, Entity& talker, Entity& listener):
    Handler(window),
    mWorld(&world),
    mTalker(&talker),
    mCurrentBranch(talker.getDialogTree()),
    mListener(&listener),
    mDialogInterface({0, 3.f * window.getSize().y / 4.f, window.getSize().x, window.getSize().y / 4.f})
{
    //assert that listener is player
    assert(listener.isPlayer());
    //assert that talker can indeed talk
    assert(mCurrentBranch);
    //initialize dialog interface
    mDialogInterface.addRelative<gr::PictureBox>("background", sf::FloatRect(0, 0, 1, 1), gTextureProvider.getMedia("dialog_bg"));
    mDialogInterface.addRelative<gr::TextBox>("dialog", sf::FloatRect(1.f/4.f, 0.f, 3.f/4.f, 1.f), gFontProvider.getMediaPtr("HighDef"), 30);
    mDialogInterface.addRelative<gr::PictureBox>("portrait", sf::FloatRect(0, 0, 1.f/4.f, 1.f), gTextureProvider.getMedia("NOGRA"));
    auto decisionsButtons = mDialogInterface.addRelative<gr::Interface>("decision_buttons", sf::FloatRect(1.f/4.f, 0, 3*1.f/4.f, 1));
    gr::Button choiceButton(sf::FloatRect(0, 0, 1.f/2.f, 1.f/2.f));
    choiceButton.addRelative<gr::PictureBox>("picture", sf::FloatRect(0, 0, 1, 1), gTextureProvider.getMedia("dialog_button"));
    decisionsButtons->addRelative("button1", choiceButton.clone());
    choiceButton.setBounds(sf::FloatRect(0, 1.f/2.f, 1.f/2.f, 1.f/2.f));
    decisionsButtons->addRelative("button2", choiceButton.clone());
    choiceButton.setBounds(sf::FloatRect(1.f/2.f, 0, 1.f/2.f, 1.f/2.f));
    decisionsButtons->addRelative("button3", choiceButton.clone());
    choiceButton.setBounds(sf::FloatRect(1.f/2.f, 1.f/2.f, 1.f/2.f, 1.f/2.f));
    decisionsButtons->addRelative("button4", choiceButton.clone());
}
//ui functions
void DialogHandler::handleEvent(const sf::Event& e)
{
    //handle the "keep talking button"
    if(e.type == sf::Event::KeyPressed) {
        if(e.key.code == sf::Keyboard::Return) {
            //update conversation
            if(mCurrentConversationIndex == mCurrentBranch->mConversation.size())
                setNext(new MatchHandler(*mWindow, mWorld));
            else
                mDialogInterface.get<gr::TextBox>("dialog").setText(mCurrentBranch->mConversation[mCurrentConversationIndex++]);
            //!WIP if conversation is over, just quit...
        }
    }
    //handle dialog events
    mDialogInterface.handleEvent(e);
}
void DialogHandler::handleLogic(const sf::Time& time)
{
    //handle only time (and *interface*) related logic, ignore turn logic while talking
    mWorld->handleTimeLogic(time);
    //update dialog interface
    mDialogInterface.update(gWindow);
}
void DialogHandler::handleDrawing()
{
    //mDialogInterface.setView(mWindow->getView());
    mWindow->setView(mWorld->getPlayer()->calcView());
    mWindow->draw(*mWorld);
    mWindow->setView(mWindow->getDefaultView());
    mWindow->draw(mDialogInterface);
}
