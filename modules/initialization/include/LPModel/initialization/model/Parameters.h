#ifndef LPMODEL_PARAMETERS_H
#define LPMODEL_PARAMETERS_H

#include "SCaBOliC/Core/ODRModel.h"
#include "SCaBOliC/Core/InterpixelSpaceHandle.h"

namespace LPModel
{
    namespace Initialization
    {
        struct Parameters
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef SCaBOliC::Core::ODRModel ODRModel;
            typedef SCaBOliC::Core::InterpixelSpaceHandle InterpixelSpaceHandle;

            Parameters(const ODRModel& odrModel,
                       const InterpixelSpaceHandle& handle,
                       const unsigned int radius=3,
                       const double squaredCurvatureWeight=1,
                       const double dataWeight=0.2):odrModel(odrModel),
                                                    handle(handle),
                                                    radius(radius),
                                                    squaredCurvatureWeight(squaredCurvatureWeight),
                                                    dataWeight(dataWeight)
            {}

            const ODRModel odrModel;
            const InterpixelSpaceHandle handle;
            const unsigned int radius;
            const double squaredCurvatureWeight;
            const double dataWeight;
        };
    }
}

#endif //LPMODEL_PARAMETERS_H
