//
// Created by vlad on 11/6/23.
//
#include "uml/angles.h"

namespace uml::angles
{
    float RadToDeg(float rads)
    {
        return rads * 180.f / 3.1415926535f;
    }

    float DegToRad(float degrees)
    {
        return degrees * 3.1415926535f / 180.f;
    }
}
