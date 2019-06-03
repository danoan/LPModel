#ifndef CREATE_ODR_IMAGE_INPUTDATA_H
#define CREATE_ODR_IMAGE_INPUTDATA_H

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

#endif //CREATE_ODR_IMAGE_INPUTDATA_H
