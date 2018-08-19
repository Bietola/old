#include "utils.h"

///functions to get random numbers
float rrange(float min, float max)
{
    assert(max > min);
    return min + ((float)rand() / (float)RAND_MAX) * (max-min);
}
float rrange(float max)
{
    return rrange(0, max);
}
