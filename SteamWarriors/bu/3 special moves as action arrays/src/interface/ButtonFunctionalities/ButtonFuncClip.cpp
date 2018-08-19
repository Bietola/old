#include "ButtonFuncClip.h"

/*
///Clip functions
//operator ()
Clip::operator()(Button& button)
{
    //make sure that button has an image
    if(!button.hasImage()){
        std::cout << "FATAL ERROR: using clip functionality on button with no image" << std::endl;
        std::terminate();
    }
    //get image dimensions
    auto image = button.getChild<PictureBox>("~image");
    auto clipDims = sf::Vector2i(image->getTexture().getSize().x / 2, image->getTexture().getSize().y);
    //update image based on click status
    if(getClickStatus() == Button::CLICKST_CLICKING){
        image->setTextureRect(sf::IntRect(clipDims.x, 0,
                                          clipDims.x, clipDims.y));
    }
    else{
        image->setTextureRect(sf::IntRect(0, 0, clipDims.x, clipDims.y));
    }
}
*/
