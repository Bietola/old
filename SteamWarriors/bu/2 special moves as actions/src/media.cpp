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
        {"stickman", "art/tilesheets/stickman.png"},
        {"halo_mon", "art/tilesheets/halo_mon.png"},
        {"tiles", "art/tilesheets/tiles.png"}
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
        {"player_idle", "art/animations/stickman.json:stickman_idle"},
        {"player_attack", "art/animations/stickman.json:stickman_attack"},
        {"halo_mon_idle", "art/animations/halo_mon.json:halo_mon_idle"}
    });
    //return
    return success;
}
