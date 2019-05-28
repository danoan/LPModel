#include "LPModel/nonlinopt/inputReader/InputData.h"

namespace LPModel
{
    namespace NonLinOpt
    {
        InputData readInput(int argc, char* argv[])
        {
            if(argc<8)
            {
                std::cerr <<"Expected pgm-input-image levels sq-weight data-weight threshold iterations output-path\n";
                exit(1);
            }

            InputData id;

            id.pgmInputImage = argv[1];
            id.levels = std::atoi(argv[2]);
            id.sqWeight = std::atof( argv[3] );
            id.dataWeight = std::atof( argv[4] );
            id.threshold = std::atof( argv[5] );
            id.iterations = std::atoi( argv[6] );
            id.outputPath = argv[7];

            return id;
        }
    }
}

