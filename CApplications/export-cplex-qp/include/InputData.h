#ifndef EXPORT_CPLEX_QP_INPUTDATA_H
#define EXPORT_CPLEX_QP_INPUTDATA_H

struct InputData
{
    enum LinearizationLevel{LINEARIZATION_NONE=0,LINEARIZATION_PIXEL_PAIR=1,LINEARIZATION_PIXEL_LINEL=2,LINEARIZATION_ALL_COUPLED=3,LINEARIZATION_ALL_UNCOUPLED=4};
    enum RelaxationLevel{RELAXATION_NONE=0,RELAXATION_ORIGINAL=1,RELAXATION_AUXILIAR=2,RELAXATION_ALL=3};

    InputData()
    {
        optRegionWidth = 1;
        sqWeight=1;
        dataWeight=1;
        linearizationLevel = LINEARIZATION_ALL_COUPLED;
        relaxationLevel = RELAXATION_NONE;
    };

    std::string pgmInputImage;
    int optRegionWidth;
    double sqWeight;
    double dataWeight;
    RelaxationLevel relaxationLevel;
    LinearizationLevel linearizationLevel;
    std::string outputPath;
};

#endif //EXPORT_CPLEX_QP_INPUTDATA_H
