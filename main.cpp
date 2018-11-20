#include <iostream>

#include "LPModel/initialization/API.h"
#include "LPModel/initialization/Shapes.h"

#include "LPModel/terms/sqc/API.h"
#include "LPModel/constraints/ClosedAndConnected.h"
#include "LPModel/lpwriter/LPWriter.h"

using namespace LPModel;
using namespace LPModel::Terms;

int main(int argc, char* argv[])
{

    Initialization::Shapes::DigitalSet ds = Initialization::Shapes::square();
    Initialization::Parameters prm = Initialization::API::initParameters(ds);

    Initialization::Grid grid = Initialization::API::createGrid(prm.odrModel.optRegion,
                                                                prm);

    SquaredCurvature::API::prepare(prm,grid);

    Constraints::ClosedAndConnected::LinelConstraints lc;
    Constraints::ClosedAndConnected::closedConnectedContraints(lc,grid);

    std::ofstream ofs("out.txt");
    LPWriter::writeConstraint(ofs,0,lc);

    return 0;
}