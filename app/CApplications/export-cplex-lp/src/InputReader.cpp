#include "InputReader.h"

InputData readInput(int argc, char* argv[])
{
    InputData in;

    int opt;
    while( (opt=getopt(argc,argv,"w:s:d:r:l:"))!=-1)
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
                std::cout << optarg << std::endl;
                if(strcmp(optarg,"none")==0) in.linearizationLevel = LPModel::LinearizationLevel::LINEARIZATION_NONE;
                else if(strcmp(optarg,"pixel-pair")==0) in.linearizationLevel = LPModel::LINEARIZATION_PIXEL_PAIR;
                else if(strcmp(optarg,"pixel-linel")==0) in.linearizationLevel = LPModel::LINEARIZATION_PIXEL_LINEL;
                else if(strcmp(optarg,"all-coupled")==0) in.linearizationLevel = LPModel::LINEARIZATION_ALL_COUPLED;
                else if(strcmp(optarg,"all-uncoupled")==0) in.linearizationLevel = LPModel::LINEARIZATION_ALL_UNCOUPLED;
                else throw std::runtime_error("Linearization option not recognized!");

                break;
            }
            case 'r':
            {
                if(strcmp(optarg,"none")==0) in.relaxationLevel = LPModel::RelaxationLevel::RELAXATION_NONE;
                else if(strcmp(optarg,"original")==0) in.relaxationLevel = LPModel::RELAXATION_ORIGINAL;
                else if(strcmp(optarg,"auxiliar")==0) in.relaxationLevel = LPModel::RELAXATION_AUXILIAR;
                else if(strcmp(optarg,"all")==0) in.relaxationLevel = LPModel::RELAXATION_ALL;
                else throw std::runtime_error("Relaxation option not recognized!");

                break;
            }
            default:
            {
                std::cerr << "Usage: pgm-input-image output-path "
                            "[-w Optimization region width. Default: 1]"
                            "[-s Squared curvature term weight. Default: 1]"
                            "[-d Data term weight. Default: 0]"
                            "[-l Linearization level {none,pixel-pair,pixel-linel,all-coupled,all-uncoupled}. Default: all-coupled";
                            "[-r Relaxation level {none,original,auxiliar,all}. Default: none";
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

std::string resolveRelaxationLevelName(LPModel::RelaxationLevel R)
{
    if(R==LPModel::RelaxationLevel::RELAXATION_NONE) return "None";
    else if(R==LPModel::RELAXATION_ORIGINAL) return "Original";
    else if(R==LPModel::RELAXATION_AUXILIAR) return "Auxiliar";
    else if(R==LPModel::RELAXATION_ALL) return "All";
    else return "Not recognized";
}

void printInputData(std::ostream& os, const InputData& id)
{
    os << "Input image: " << id.pgmInputImage << "\n"
       << "Optimization region width: " << id.optRegionWidth << " \n"
       << "Squared Curvature Weight: " << id.sqWeight << " \n"
       << "Data Weight: " << id.dataWeight << " \n"
       << "Relaxation Level: " << resolveRelaxationLevelName(id.relaxationLevel)<< " \n"
       << "Linearization Level: " << resolveLinearizationLevelName(id.linearizationLevel)<< " \n";
}