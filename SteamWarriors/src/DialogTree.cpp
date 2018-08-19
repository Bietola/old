#include "DialogTree.h"

///static variables
MediaProvider<DialogTree> DialogTree::models;


///functions
//constructors
DialogTree::DialogTree():
    mVisitPriority(0),
    mConversation(),
    mDecisionText(),
    mChildren(),
    mIsVisitBranch(false),
    mIsDecisionBranch(false)
{}
DialogTree::DialogTree(const Json::Value& root, DialogTree::LoadingMode loadingMode):
    DialogTree()
{
    assert(loadFromJsonObject(root, loadingMode));
}
//destructor
DialogTree::~DialogTree() {
    /*for(auto ele : mChildren) {
        delete ele;
    }*/
    /*!REMEMBER TO DELETE ACTIONS*/
}
//load all models
bool DialogTree::loadModels() {
    return models.loadMediaFromDirectory("./data/dialogs/");
}
//load from json node - return success
bool DialogTree::loadFromJsonObject(const Json::Value& root, DialogTree::LoadingMode loadingMode) {
    assert(root.isObject());
    //search for special stuffs
    for(auto itr = root.begin(); itr != root.end(); ++itr) {
        //if in decisions mode, add the name of the node as the decision text of the new branch
        if(loadingMode == LoadingMode::Decisions) {
            auto child = new DialogTree(*itr);
            child->mDecisionText = itr.name();
            mChildren.push_back(child);
        }
        //if in visitor mode, add the name of the node as the visit level of the new branch
        else if(loadingMode == LoadingMode::Visits) {
            auto child = new DialogTree(*itr);
            child->mVisitPriority = std::atoi(itr.name().c_str());
            mChildren.push_back(child);
        }
        //if in default mode, do stuff...
        else {
            //decision - add decision title
            if(itr.name() == "decisions") {
                auto child = new DialogTree(*itr, LoadingMode::Decisions);
                child->mIsDecisionBranch = true;
                mChildren.push_back(child);
            }
            //visit - add visit priority
            else if(itr.name() == "visits") {
                auto child = new DialogTree(*itr, LoadingMode::Visits);
                child->mIsVisitBranch = true;
                mChildren.push_back(child);
            }
            //event - add an event
            else if(itr.name() == "event") {
                //!WIP
            }
            //text, add it all to the conversation
            else if(itr.name() == "dialog") {
                //add single line of conversation
                if(itr->isString()) {
                    std::cout << "STRING:" << itr->asString() << std::endl;
                    mConversation.push_back(itr->asString());
                }
                //add multiple lines of conversation
                else if(itr->isArray()) {
                    for(size_t j = 0; j < itr->size(); j++) {
                        std::cout << "ARR: " << (*itr)[j].asString() << std::endl;
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
                std::cout << "FATAL ERROR: \"" << itr.name() << "\" could not be recognized as a dialog type" << std::endl;
                assert(false);
            }
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


///DialogIterator functions
//constructor
DialogIterator::DialogIterator(const DialogTree* dialogTree):
    mDialogTree(dialogTree)
{}

//get text
std::string DialogIterator::getText() const {
    assert(mDialogTree);
    if(mDialogTree->mConversation.empty())
        return "";
    else {
        std::cout << "index: " << mCurrentConversationIndex << std::endl;
        std::cout << mDialogTree->mConversation.size() << " " << mDialogTree->mConversation.empty() << std::endl;
        assert(mCurrentConversationIndex < mDialogTree->mConversation.size());
        return mDialogTree->mConversation[mCurrentConversationIndex];
    }
}

//go to next dialog text or to next dialog branch
bool DialogIterator::next(const std::string& decisionText, unsigned int visitPriority) {
    //advance in dialog, go to next branch if dialog is over
    if(++mCurrentConversationIndex >= mDialogTree->mConversation.size()) {
        mCurrentConversationIndex = 0;
        //if current branch contains decisions, pick the right one
        if(mDialogTree->isDecisionBranch()) {
            for(const auto ele : mDialogTree->mChildren) {
                if(ele->getDecisionText() == decisionText) {
                    mDialogTree = ele;
                    return true;
                }
            }
            //if no decision is found, assert (for now)
            assert(false);
        }
        //if visits, pick the right visit value
        else if(mDialogTree->isVisitBranch()) {
            for(const auto ele : mDialogTree->mChildren) {
                if(ele->getVisitPriority() == visitPriority) {
                    mDialogTree = ele;
                    return true;
                }
            }
            //if no correct visit priority is found, assert (for now)
            assert(false);
        }
        //if the branch is not special at all, just choose the first child
        else if(!mDialogTree->mChildren.empty()) {
            mDialogTree = mDialogTree->mChildren[0];
            return true;
        }
        //if there are no children, consider it the end of the tree
        else
            return false;
    }
    else return true;
}

//go on until a branch with available text is reached and return it (or return an empty string if the branches run out first)
std::string DialogIterator::getNextText(unsigned int visitPriority) {
    std::string text;
    do {
        if(!next("", visitPriority)) return "";
    } while((text = getText()).empty());
    return text;
}
