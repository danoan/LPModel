#ifndef EXPORT_CPLEX_QP_INPUTREADER_H
#define EXPORT_CPLEX_QP_INPUTREADER_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdexcept>

#include "InputData.h"

InputData readInput(int argc, char* argv[]);
std::string resolveRelaxationLevelName(InputData::RelaxationLevel R);
std::string resolveLinearizationLevelName(InputData::LinearizationLevel L);

#endif //EXPORT_CPLEX_LP_INPUTREADER_H
