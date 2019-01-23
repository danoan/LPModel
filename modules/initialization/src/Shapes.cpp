#include "LPModel/initialization/Shapes.h"

using namespace LPModel::Initialization;

Shapes::DigitalSet Shapes::square(double h)
{
    NGon2D square(0,0,6,4,3.1416/4.0);
    DigitalSet ds = Internal::shapeDigitizer(square,h);

    return DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(ds);
}

Shapes::DigitalSet Shapes::flower(double h)
{
    Flower flower(0,0,10,5,3,0);
    DigitalSet ds = Internal::shapeDigitizer(flower,h);

    return DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(ds);
}