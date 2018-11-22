#include "LPModel/initialization/Shapes.h"

using namespace LPModel::Initialization;

Shapes::DigitalSet Shapes::square()
{
    NGon2D square(0,0,10,4,3.1416/4.0);
    DigitalSet ds = Internal::shapeDigitizer(square,0.5);

    return DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(ds);
}

Shapes::DigitalSet Shapes::flower()
{
    Flower flower(0,0,10,5,3,0);
    DigitalSet ds = Internal::shapeDigitizer(flower,0.5);

    return DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(ds);
}