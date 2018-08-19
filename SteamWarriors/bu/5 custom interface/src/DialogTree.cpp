#include "DialogTree.h"

///static variables
MediaProvider<DialogTree> DialogTree::models;


///functions
//load all models
bool DialogTree::loadModels() {
    return models.loadMediaFromDirectory("./data/dialogs/");
}
//load from json node - return success
bool DialogTree::loadFromJsonObject(const Json::Value& root) {
    //search for special stuff
    for(auto itr = root.begin(); itr != root.end(); ++itr) {
        //branch - add object as child
        if(itr.name() == "branch") {
            //load another child tree with the nameless object
            assert(itr->isObject());
            auto child = new DialogTree;
            assert(child->loadFromJsonObject(*itr));
            mChildren.push_back(child);
        }
        //decision - add decision title
        else if(itr.name() == "decision") {
            assert(itr->isString());
            mDecisionText = itr->asString();
        }
        //visit - add visit priority
        else if(itr.name() == "visit") {
            assert(itr->isIntegral());
            mVisitPriority = itr->asInt();
        }
        //event - add an event
        else if(itr.name() == "event") {
            //!WIP
        }
        //text, add it all to the conversation
        else if(itr.name() == "text" || itr.name() == "you") {
            //add single line of conversation
            if(itr->isString()) {
                mConversation.push_back(itr->asString());
            }
            //add multiple lines of conversation
            else if(itr->isArray()) {
                for(size_t j = 0; j < itr->size(); j++) {
                    assert((*itr)[j].isString());
                    mConversation.push_back((*itr)[j].asString());
                }
            }
            //else there must be some stupid error...
            else
                assert(false);
        }
        //else just return false
        else{
            assert(false);
        }
    }
    //!WIPPISH
    return true;
}
//load from a json file - return if success
bool DialogTree::loadFromFile(const std::string& path) {
    std::ifstream file;
    file.open(path.c_str(), std::ifstream::binary);
    Json::Value root;
    file >> root;
    return loadFromJsonObject(root);
}
