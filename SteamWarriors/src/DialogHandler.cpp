#include "DialogHandler.h"

///DialogHandler functions
//constructor
DialogHandler::DialogHandler(sf::RenderWindow& window, World& world, Entity& talker, Entity& listener):
    Handler(window),
    mWorld(&world),
    mTalker(&talker),
    mListener(&listener),
    mDialogInterface(window),
    mCurrentDialog(talker.getDialogTree())
{
    //assert that listener is player
    assert(listener.isPlayer());
    //increase number of visits of talking entity
    mTalker->addVisit();
    //initialize dialog interface
    auto dialogPanel = std::make_shared<tgui::Panel>();
    dialogPanel->setPosition(0, mWindow->getSize().y / 4.f * 3.f);
    dialogPanel->setSize(mWindow->getSize().x, mWindow->getSize().y / 4.f);
    auto dialogBox = std::make_shared<tgui::TextBox>();
    dialogBox->setFont(gFontProvider.getMedia("HighDef"));
    dialogBox->setSize("parent.width / 4 * 3", "parent.height");
    dialogBox->setPosition(1,1); //!WORKAROUND NEEDED FOR BUG
    dialogBox->setPosition("parent.width / 4", 0);
    dialogPanel->add(dialogBox, "dialog");
    mDialogInterface.add(dialogPanel, "panel");
    updateConversation();
}
//ui functions
void DialogHandler::handleEvent(const sf::Event& e)
{
    //handle the "keep talking button"
    if(e.type == sf::Event::KeyPressed) {
        if(e.key.code == sf::Keyboard::Return) {
            updateConversation();
        }
    }
    //handle dialog events
    mDialogInterface.handleEvent(e);
}
void DialogHandler::handleLogic(const sf::Time& time)
{
    //handle only time (and *interface*) related logic, ignore turn logic while talking
    mWorld->handleTimeLogic(time);
}
void DialogHandler::handleDrawing()
{
    //mDialogInterface.setView(mWindow->getView());
    mWindow->setView(mWorld->getPlayer()->calcView());
    mWindow->draw(*mWorld);
    mWindow->setView(mWindow->getDefaultView());
    mDialogInterface.draw();
}

//update conversation
void DialogHandler::updateConversation() {
    auto dialogText = mCurrentDialog.getNextText(mTalker->getNumberOfVisits());
    if(dialogText.empty())
        setNext(new MatchHandler(*mWindow, mWorld));
    else
        mDialogInterface.get<tgui::Panel>("panel")->get<tgui::TextBox>("dialog")->setText(dialogText);
}
