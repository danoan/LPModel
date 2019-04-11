#ifndef EXPORT_CVXOPT_INPUTDATA_H
#define EXPORT_CVXOPT_INPUTDATA_H

#include "LPModel/types/types.h"

struct InputData
{
    InputData()
    {
        optRegionWidth = 1;
        sqWeight=1;
        dataWeight=1;
        linearizationLevel = LPModel::LINEARIZATION_ALL_COUPLED;
    };

    std::string pgmInputImage;
    int optRegionWidth;
    double sqWeight;
    double dataWeight;
    LPModel::LinearizationLevel linearizationLevel;
    std::string outputPath;
};

#endif //EXPORT_CVXOPT_INPUTDATA_H
