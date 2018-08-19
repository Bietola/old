#ifndef DIALOGTREE_H_INCLUDED
#define DIALOGTREE_H_INCLUDED

#include <cassert>
#include <vector>

#include <json/json.h>

#include "MediaProvider.h"

///DialogTree class
class DialogTree {
    public:
        ///friends
        friend class DialogIterator;
        ///enumerations
        enum class LoadingMode {
            Default,
            Visits,
            Decisions
        };
        ///constructors
        DialogTree();
        DialogTree(const Json::Value& root, LoadingMode loadingMode = LoadingMode::Default);
        ///destructor
        ~DialogTree();
        ///static functions
        static const MediaProvider<DialogTree>& getModels() {return models;}
        static bool loadModels();
        ///getters
        unsigned int getVisitPriority() const {return mVisitPriority;}
        std::string getDecisionText() const {return mDecisionText;}
        ///checkers
        bool isVisitBranch() const {return mIsVisitBranch;}
        bool isDecisionBranch() const {return mIsDecisionBranch;}
        ///load functions
        //load from json node - return success
        bool loadFromJsonObject(const Json::Value& root, LoadingMode loadingMode = LoadingMode::Default);
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
        bool mIsVisitBranch = false;
        bool mIsDecisionBranch = false;
};

///Dialog Iterator class
class DialogIterator {
    public:
        ///constructor
        DialogIterator(const DialogTree* root);
        ///destructor
        ~DialogIterator() {}
        ///functions
        std::string getText() const;
        std::string getNextText(unsigned int visitPriority = 0);
        bool next(const std::string& decisionText = "", unsigned int visitPriority = 0); //returns if the dialog tree is over
    private:
        ///variables
        const DialogTree* mDialogTree;
        size_t mCurrentConversationIndex = 0;
};

#endif // DIALOGUE_H_INCLUDED
