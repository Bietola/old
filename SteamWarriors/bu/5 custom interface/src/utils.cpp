#include "utils.h"

///functions to get random numbers
int rrange(int min, int max) {
    assert(max > min);
    return min + rand()%(max - min + 1);
}
float rrangef(float min, float max) {
    assert(max > min);
    return min + ((float)rand() / (float)RAND_MAX) * std::abs(max-min);
}
int rrange(int max) {
    return rrange(0, max);
}
float rrangef(float max) {
    return rrangef(0, max);
}

