#include <iostream>

#include "LPModel/initialization/API.h"
#include "LPModel/initialization/Shapes.h"

#include "LPModel/terms/sqc/API.h"

using namespace LPModel;
using namespace LPModel::Terms;

int main(int argc, char* argv[])
{

    Initialization::Shapes::DigitalSet ds = Initialization::Shapes::square();
    Initialization::Parameters prm = Initialization::API::initParameters(ds);

    Initialization::Grid grid = Initialization::API::createGrid(prm.pixelOptRegion);

    SquaredCurvature::API::prepare(prm,grid);

    return 0;
}