#ifndef EXPORT_CPLEX_QP_INPUTDATA_H
#define EXPORT_CPLEX_QP_INPUTDATA_H

#include "LPModel/types/types.h"

struct InputData
{
    InputData()
    {
        optRegionWidth = 1;
        sqWeight=1;
        dataWeight=1;
        linearizationLevel = LPModel::LINEARIZATION_ALL_COUPLED;
        relaxationLevel = LPModel::RELAXATION_NONE;
    };

    std::string pgmInputImage;
    std::string gridObjectFile;
    int optRegionWidth;
    double sqWeight;
    double dataWeight;
    LPModel::RelaxationLevel relaxationLevel;
    LPModel::LinearizationLevel linearizationLevel;
    std::string outputPath;
};

#endif //EXPORT_CPLEX_QP_INPUTDATA_H
