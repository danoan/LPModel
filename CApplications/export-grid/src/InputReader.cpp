#include "InputReader.h"

std::string pgmInputImage;
int optRegionWidth;
std::string outputPath;

InputData readInput(int argc, char* argv[])
{
    InputData in;
    int opt;
    while( (opt=getopt(argc,argv,"w:"))!=-1 )
    {
        switch(opt)
        {
            case 'w':
            {
                in.optRegionWidth = atoi(optarg);
                break;
            }
            default:
            {
                std::cerr << "Usage: PGMShapeImage OutputFilePath [-w OptRegionWidth default: 1]\n";
                exit(1);
            }
        }
    }

    in.pgmInputImage = argv[optind++];
    in.outputPath = argv[optind++];

    return in;
}