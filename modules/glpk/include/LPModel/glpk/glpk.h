#ifndef LPMODEL_GLPK_H
#define LPMODEL_GLPK_H

#include <string>
#include <fstream>
#include <iostream>

#include <glpk.h>

namespace LPModel
{
    namespace GLPK
    {
        void glpk_solver(const std::string& lpInputFile, const std::string& outputSolutionFile);
    }
}

#endif //LPMODEL_GLPK_H
