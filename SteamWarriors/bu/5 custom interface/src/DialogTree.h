#ifndef DIALOGTREE_H_INCLUDED
#define DIALOGTREE_H_INCLUDED

#include <cassert>
#include <vector>

#include <json/json.h>

#include "MediaProvider.h"
#include "actions.h"

///DialogTree class
class DialogTree {
    public:
        ///friends
        friend class DialogHandler;
        ///constructor
        DialogTree() {}
        ///destructor
        ~DialogTree() {/*!REMEMBER TO DELETE ACTIONS*/}
        ///static functions
        static const MediaProvider<DialogTree>& getModels() {return models;}
        static bool loadModels();
        ///other functions
        //load from json node - return success
        bool loadFromJsonObject(const Json::Value& root);
        //load from a json file - return if success
        bool loadFromFile(const std::string& path);
    private:
        //models
        static MediaProvider<DialogTree> models;
        //variables
        unsigned int mVisitPriority = 0;
        std::vector<std::string> mConversation;
        std::string mDecisionText;
        std::vector<DialogTree*> mChildren;
};

#endif // DIALOGUE_H_INCLUDED
