#include <iostream>
#include "LPModel/initialization/API.h"
#include "LPModel/initialization/Shapes.h"


int main(int argc, char* argv[])
{
    using namespace LPModel::Initialization;
    Shapes::DigitalSet ds = Shapes::square();
    ODRModel odrModel = API::createOdrModel(ds);

    Grid grid = API::createGrid(odrModel.optRegion);


    return 0;
}