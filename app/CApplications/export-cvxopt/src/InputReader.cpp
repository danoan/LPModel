#include "InputReader.h"

InputData readInput(int argc, char* argv[])
{
    InputData in;

    int opt;
    while( (opt=getopt(argc,argv,"w:s:d:l:"))!=-1)
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
            case 'l':
            {
                if(strcmp(optarg,"none")==0) in.linearizationLevel = LPModel::LinearizationLevel::LINEARIZATION_NONE;
                else if(strcmp(optarg,"pixel-pair")==0) in.linearizationLevel = LPModel::LINEARIZATION_PIXEL_PAIR;
                else if(strcmp(optarg,"pixel-linel")==0) in.linearizationLevel = LPModel::LINEARIZATION_PIXEL_LINEL;
                else if(strcmp(optarg,"all-coupled")==0) in.linearizationLevel = LPModel::LINEARIZATION_ALL_COUPLED;
                else if(strcmp(optarg,"all-uncoupled")==0) in.linearizationLevel = LPModel::LINEARIZATION_ALL_UNCOUPLED;
                else throw std::runtime_error("Linearization option not recognized!");

                break;
            }
            default:
            {
                std::cerr << "Usage: pgm-input-image output-path "
                            "[-w Optimization region width. Default: 1]"
                            "[-s Squared curvature term weight. Default: 1]"
                            "[-d Data term weight. Default: 0]"
                            "[-l Linearization level {none,pixel-pair,pixel-linel,all-coupled,all-uncoupled}. Default: all-coupled";
                exit(1);
            }
        }
    }

    in.pgmInputImage = argv[optind++];
    in.outputPath = argv[optind++];

    return in;
}

std::string resolveLinearizationLevelName(LPModel::LinearizationLevel L)
{
    if(L==LPModel::LinearizationLevel::LINEARIZATION_NONE) return "None";
    else if(L==LPModel::LINEARIZATION_PIXEL_PAIR) return "Pixel-Pair";
    else if(L==LPModel::LINEARIZATION_PIXEL_LINEL) return "Pixel-Linel";
    else if(L==LPModel::LINEARIZATION_ALL_COUPLED) return "All-Coupled";
    else if(L==LPModel::LINEARIZATION_ALL_UNCOUPLED) return "All-Uncoupled";
    else return "Not recognized";
}
