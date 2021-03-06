#ifndef MEDIA_H_INCLUDED
#define MEDIA_H_INCLUDED

#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "MediaProvider.h"

///global objects and variables
extern MediaProvider<sf::Texture> gTextureProvider;
extern MediaProvider<sf::Font> gFontProvider;
extern MediaProvider<AnimationInfo> gAnimationProvider;

///global functions
//load all media, return false on failure
extern bool load_media();

#endif // MEDIA_H_INCLUDED
