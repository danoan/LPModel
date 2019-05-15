#ifndef EXPORT_GRID_INPUTDATA_H
#define EXPORT_GRID_INPUTDATA_H

struct InputData
{
    InputData()
    {
        optRegionWidth = 1;
    };

    std::string pgmInputImage;
    int optRegionWidth;
    std::string outputPath;
};

#endif //EXPORT_GRID_INPUTDATA_H
