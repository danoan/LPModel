#ifndef LPMODEL_INPUTDATA_H
#define LPMODEL_INPUTDATA_H

#include <iostream>
#include <string>

namespace LPModel
{
    namespace NonLinOpt
    {
        struct InputData
        {
            std::string pgmInputImage;
            int levels;
            double sqWeight;
            double dataWeight;
            double threshold;
            int iterations;
            std::string outputPath;
        };

        InputData readInput(int argc, char* argv[]);
    }
}

#endif //LPMODEL_INPUTDATA_H
