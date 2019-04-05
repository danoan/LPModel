#ifndef EXPORT_CVXOPT_INPUTREADER_H
#define EXPORT_CVXOPT_INPUTREADER_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdexcept>

#include "InputData.h"

InputData readInput(int argc, char* argv[]);
std::string resolveRelaxationLevelName(InputData::RelaxationLevel R);
std::string resolveLinearizationLevelName(InputData::LinearizationLevel L);

#endif //EXPORT_CVXOPT_INPUTREADER_H
