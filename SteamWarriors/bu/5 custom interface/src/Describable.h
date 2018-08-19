#ifndef DESCRIBABLE_H_INCLUDED
#define DESCRIBABLE_H_INCLUDED

#include <string>

class Describable {
    public:
        const std::string& getName() const {return mName;}
        const std::string& getDescription() const {return mDescription;}
        void setName(const std::string& name) {mName = name;}
        void setDescription(const std::string& description) {mDescription = description;}
        Describable():
            mName("NONAME"), mDescription("NODESCIPTION") {}
        Describable(const std::string& name, const std::string& description):
            mName(name), mDescription(description) {}
    protected:
        std::string mName, mDescription;
};

#endif // DESCRIBABLE_H_INCLUDED
