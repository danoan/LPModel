#include "LPModel/initialization/Shapes.h"

using namespace LPModel::Initialization;

Shapes::DigitalSet Shapes::square()
{
    NGon2D square(0,0,10,4,3.1416/4.0);
    GaussDigitizer gd;

    gd.init(square.getLowerBound(),square.getUpperBound(),1.0);
    gd.attach(square);

    Domain domain(square.getLowerBound(),square.getUpperBound());
    DigitalSet ds(domain);
    DGtal::Shapes<Domain>::digitalShaper(ds,gd);

    return DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(ds);
}