#include "media.h"

///global objects and variables
//media providers
MediaProvider<sf::Texture> gTextureProvider;
MediaProvider<sf::Font> gFontProvider;
MediaProvider<AnimationInfo> gAnimationProvider;

///global functions
//load all media, return false on failure
bool load_media()
{
    //success/failure flag
    bool success = true;
    //load textures
    success &= gTextureProvider.loadMedia(
    {
        {"characters", "art/characters.png"},
        {"tiles", "art/tiles.png"}
    });
    //load fonts
    success &= gFontProvider.loadMedia(
    {
        {"message", "fonts/LinLibertine_DRah.ttf"},
        {"title", "fonts/GoodDog.otf"}
    });
    //load animations
    success &= gAnimationProvider.loadMedia(
    {
        {"player_idle", "WIP"}
    });
    //return
    return success;
}
