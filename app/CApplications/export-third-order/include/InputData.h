#ifndef EXPORT_THIRD_ORDER_INPUTDATA_H
#define EXPORT_THIRD_ORDER_INPUTDATA_H

struct InputData
{
    InputData()
    {
        optRegionWidth = 1;
        sqWeight=1;
        dataWeight=1;
        binaryPenalty = false;
    };

    std::string pgmInputImage;
    int optRegionWidth;
    double sqWeight;
    double dataWeight;
    bool binaryPenalty;
    std::string outputPath;
};

#endif //EXPORT_THIRD_ORDER_INPUTDATA_H
