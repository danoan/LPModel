#include "InputReader.h"

InputData readInput(int argc, char* argv[])
{
    InputData in;

    int opt;
    while( (opt=getopt(argc,argv,"w:s:d:b"))!=-1)
    {
        switch(opt)
        {
            case 'w':
            {
                in.optRegionWidth = std::atoi(optarg);
                break;
            }
            case 's':
            {
                in.sqWeight = std::atof( optarg );
                break;
            }
            case 'd':
            {
                in.dataWeight = std::atof( optarg );
                break;
            }
            case 'b':
            {
                in.binaryPenalty = true;
                break;
            }
            default:
            {
                std::cerr << "Usage: pgm-input-image output-path "
                            "[-w Optimization region width. Default: 1]"
                            "[-s Squared curvature term weight. Default: 1]"
                            "[-d Data term weight. Default: 0]"
                            "[-b Enforce binary variables]";
                exit(1);
            }
        }
    }

    in.pgmInputImage = argv[optind++];
    in.outputPath = argv[optind++];

    return in;
}
