#ifndef LPMODEL_INITIALIZATION_PARAMETERS_H
#define LPMODEL_INITIALIZATION_PARAMETERS_H

#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Core/ODRPixels/PixelSpaceHandle.h"

namespace LPModel
{
    namespace Initialization
    {
        struct Parameters
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef SCaBOliC::Core::ODRModel ODRModel;
            typedef SCaBOliC::Core::PixelSpaceHandle PixelSpaceHandle;

            Parameters(const ODRModel& odrModel,
                       const PixelSpaceHandle& handle,
                       const unsigned long radius=3,
                       const double squaredCurvatureWeight=1,
                       const double dataWeight=0.2):odrModel(odrModel),
                                                    handle(handle),
                                                    radius(radius),
                                                    squaredCurvatureWeight(squaredCurvatureWeight),
                                                    dataWeight(dataWeight)
            {}

            const ODRModel odrModel;
            const PixelSpaceHandle handle;
            const unsigned long radius;
            const double squaredCurvatureWeight;
            const double dataWeight;
        };
    }
}

#endif //LPMODEL_INITIALIZATION_PARAMETERS_H
