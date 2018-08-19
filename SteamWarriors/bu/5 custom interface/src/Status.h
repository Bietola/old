#ifndef STATUS_H_INCLUDED
#define STATUS_H_INCLUDED

#include <iostream>
#include <string>
#include <map>

struct Status {
public:
        //friends
        friend class Entity;
        //constructors
        Status() {}
        Status(const std::initializer_list<std::pair<std::string, double>>& stats);
        Status(const Status& status);
        //destructor
        ~Status() {}
        //getters
        double get(const std::string& key) const;
        //operators
        double& operator[](const std::string& key) {return mStats[key];}
        double operator[](const std::string& key) const;
        Status& operator=(const Status& lhs);
        Status operator*(const Status& lhs);
        Status operator+(const Status& lhs);
        Status operator-(const Status& lhs);
        Status& operator*=(const Status& lhs);
        Status& operator+=(const Status& lhs);
        Status& operator-=(const Status& lhs);
    private:
        //stats
        std::map<std::string, double> mStats;
};


#endif
