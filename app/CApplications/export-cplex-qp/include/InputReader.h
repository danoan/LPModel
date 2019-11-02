#ifndef EXPORT_CPLEX_QP_INPUTREADER_H
#define EXPORT_CPLEX_QP_INPUTREADER_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdexcept>

#include "InputData.h"
#include "LPModel/types/types.h"

void usage(int argc, char* argv[]);
InputData readInput(int argc, char* argv[]);
std::string resolveRelaxationLevelName(LPModel::RelaxationLevel R);
std::string resolveLinearizationLevelName(LPModel::LinearizationLevel L);

#endif //EXPORT_CPLEX_LP_INPUTREADER_H
