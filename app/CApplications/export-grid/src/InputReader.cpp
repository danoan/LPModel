#include "InputReader.h"

std::string pgmInputImage;
int optRegionWidth;
std::string outputPath;

void usage(int argc, char* argv[])
{
    std::cerr << "Usage: PGMShapeImage OutputFilePath [-w OptRegionWidth default: 1]\n";
}

InputData readInput(int argc, char* argv[])
{
    if (argc < 3)
    {
        usage(argc,argv);
        exit(1);
    }

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
                usage(argc,argv);
                exit(1);
            }
        }
    }

    in.pgmInputImage = argv[optind++];
    in.outputPath = argv[optind++];

    return in;
}