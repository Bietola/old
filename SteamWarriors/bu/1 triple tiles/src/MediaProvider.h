#ifndef MEDIAPROVIDER_H_INCLUDED
#define MEDIAPROVIDER_H_INCLUDED

#include <iostream>
#include <map>

#include <SFML/Graphics.hpp>

///MediaProvider class
template<typename T>
class MediaProvider
{
public:
    ///aliases
    using StringPair = std::pair<std::string, std::string>;
    ///media loading functions
    //load it
    bool loadMedia(std::initializer_list<StringPair> paths);
    ///provider functions
    //get it
    const T& getMedia(std::string key) const;
    const T* getMediaPtr(std::string key) const;
private:
    ///variables
    //textures
    std::map<std::string, T> mMediaContainer;
};

///MediaProvider function templates
//load generic media object into a media container
template<typename T>
bool MediaProvider<T>::loadMedia(std::initializer_list<MediaProvider::StringPair> paths)
{
    //return value
    bool ret = true;
    //attempt to initialize media one by one
    for(const auto& ele : paths)
    {
        //tmp
        T media;
        //load media
        if(ret &= media.loadFromFile(ele.second))
        {
            //add media to media container
            mMediaContainer[ele.first] = media;
        }
        else
        {
            //errors in the loading process should be handled by the media loading function
            ;
        }
    }
    //return success
    return ret;
}
//retrieve a generic media object from a container
template<typename T>
const T& MediaProvider<T>::getMedia(std::string key) const
{
    //attempt to retrieve media by key
    try
    {
        return mMediaContainer.at(key);
    }
    catch(std::out_of_range)
    {
        //terminate program
        std::cout << "FATAL ERROR: trying to access nonexistent key through media provider (key = " << key << ")." << std::endl;
        std::terminate();
    }
}
template<typename T>
const T* MediaProvider<T>::getMediaPtr(std::string key) const
{
    return &getMedia(key);
}


#endif // MEDIAPROVIDER_H_INCLUDED
