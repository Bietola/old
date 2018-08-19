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
    /*!DEPRECATED
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
    */
    bool success = true;
    //load textures
    success &= gTextureProvider.loadMediaFromDirectory(fs::path("./art/"));
    //load fonts
    success &= gFontProvider.loadMediaFromDirectory(fs::path("./fonts/"));
    //load animations
    success &= gAnimationProvider.loadMediaFromDirectory(fs::path("./data/animations/"), '_');
    //
    return success;
}
