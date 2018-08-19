#ifndef MEDIAPROVIDER_H_INCLUDED
#define MEDIAPROVIDER_H_INCLUDED

#include <iostream>
#include <map>
#include <boost/optional.hpp>
#include <boost/filesystem.hpp>

#include <SFML/Graphics.hpp>

namespace fs = boost::filesystem;

///MediaProvider class
template<typename T>
class MediaProvider {
public:
    ///aliases
    using StringPair = std::pair<std::string, std::string>;
    ///statics
    //temp access media counter
    unsigned int tempCounter = 0;
    bool tempCreationAllowed = false;
    ///media loading functions
    //add it
    void addMedia(const std::string& key, const T& media);
    //load it
    bool loadMedia(std::initializer_list<StringPair> paths);
    bool loadMedia(const std::string& key, const std::string& path);
    //add it as temporary
    std::string addTempMedia(const T& media);
    //load it recursively from a directory
    //  generate keys from the file name and (optionally) from the folder in which the file is stored
    bool loadMediaFromDirectory(const fs::path& dirPath, char seprator = '\0');
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
//add generic media object into a media container
template<typename T>
void MediaProvider<T>::addMedia(const std::string& key, const T& media) {
    //paths starting with an underscore are reserved for temporary assets (and are ergo invalid)
    assert(!key.empty() && (key[0] != '_' || tempCreationAllowed));
    //add media
    mMediaContainer[key] = media;
}
//load generic media object into a media container
template<typename T>
bool MediaProvider<T>::loadMedia(std::initializer_list<MediaProvider::StringPair> paths) {
    //return value
    bool ret = true;
    //attempt to initialize media one by one
    for(const auto& ele : paths)
    {
        //load media
        ret &= loadMedia(ele.first, ele.second);
    }
    //return success
    return ret;
}
template<typename T>
bool MediaProvider<T>::loadMedia(const std::string& key, const std::string& path) {
    //paths starting with an underscore are reserved for temporary assets (and are ergo invalid)
    assert(!key.empty() && (key[0] != '_' || tempCreationAllowed));
    //tmp
    T media;
    //load media
    if(media.loadFromFile(path)) {
        //add media to media container
        mMediaContainer[key] = media;
        //success
        return true;
    }
    else {
        //errors in the loading process should be handled by the media loading function
        ;
        //failure
        return false;
    }
}
//add object as a temporary access asset and return the temporary access key
template<typename T>
std::string MediaProvider<T>::addTempMedia(const T& media) {
    tempCreationAllowed = true;
    auto key = std::string("_") + std::to_string(tempCounter++);
    addMedia(key, media);
    tempCreationAllowed = false;
    return key;
}
//load it recursively from a directory
template<typename T>
bool MediaProvider<T>::loadMediaFromDirectory(const fs::path& dirPath, char separator) {
    for(fs::directory_entry& dir : fs::recursive_directory_iterator(dirPath)) {
        if(fs::is_regular(dir)){
            std::string key = dir.path().filename().generic_string();
            key = key.substr(0, key.find('.'));
            std::cout << key << std::endl;

            if(separator != '\0') {
                assert(dir.path().has_branch_path());
                key = dir.path().branch_path().leaf().generic_string() + separator + key;
            }

            loadMedia(key, dir.path().generic_string());
        }
    }
}
//retrieve a generic media object from a container
template<typename T>
const T& MediaProvider<T>::getMedia(std::string key) const {
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
const T* MediaProvider<T>::getMediaPtr(std::string key) const {
    return &getMedia(key);
}


#endif // MEDIAPROVIDER_H_INCLUDED
