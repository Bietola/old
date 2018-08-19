#include "Status.h"

//constructors
Status::Status(const std::initializer_list<std::pair<std::string, double>>& stats)
{
    for(const auto& ele : stats) {
        mStats.insert(ele);
    }
}

Status::Status(const Status& status) {
    mStats = status.mStats;
}

//operators
const double& Status::operator[](const std::string& key) const {
    try {
        return mStats.at(key);
    }
    catch(std::out_of_range) {
        std::cout << "nonexistent status key used to retrieve stat (key = " << key << ")" << std::endl;
    }
}
Status& Status::operator=(const Status& lhs) {
    mStats = lhs.mStats;
    return *this;
}

Status Status::operator*(const Status& lhs) {
    Status res;
    for(const auto& ele : lhs.mStats) {
        if(mStats.find(ele.first) != mStats.end()) {
            res[ele.first] *= ele.second;
        }
    }
    return res;
}

Status Status::operator+(const Status& lhs) {
    Status res;
    for(const auto& ele : lhs.mStats) {
        if(mStats.find(ele.first) != mStats.end()) {
            res[ele.first] += ele.second;
        }
    }
    return res;
}

Status Status::operator-(const Status& lhs) {
    Status res;
    for(const auto& ele : lhs.mStats) {
        if(mStats.find(ele.first) != mStats.end()) {
            res[ele.first] -= ele.second;
        }
    }
    return res;
}

Status& Status::operator*=(const Status& lhs) {
    return *this = *this * lhs;
}

Status& Status::operator+=(const Status& lhs) {
    return *this = *this + lhs;
}

Status& Status::operator-=(const Status& lhs) {
    return *this = *this - lhs;
}
