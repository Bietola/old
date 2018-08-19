#ifndef DESCRIBABLE_H_INCLUDED
#define DESCRIBABLE_H_INCLUDED

#include <string>

class Describable {
    public:
        const std::string& getName() const {return mName;}
        const std::string& getDescription() const {return mDescription;}
        Describable(const std::string& name, const std::string& description):
            mName(name), mDescription(description) {}
    private:
        std::string mName, mDescription;
};

#endif // DESCRIBABLE_H_INCLUDED
