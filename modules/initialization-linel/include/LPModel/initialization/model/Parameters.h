#ifndef LPMODEL_INITIALIZATIONLINEL_PARAMETERS_H
#define LPMODEL_INITIALIZATIONLINEL_PARAMETERS_H

#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Core/ODRLinels/LinelSpaceHandle.h"

namespace LPModel
{
    namespace Initialization
    {
        struct Parameters
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef SCaBOliC::Core::ODRModel ODRModel;
            typedef SCaBOliC::Core::LinelSpaceHandle LinelSpaceHandle;

            Parameters(const ODRModel& odrModel,
                       const LinelSpaceHandle& handle,
                       const unsigned long radius=3,
                       const double squaredCurvatureWeight=1,
                       const double dataWeight=0.2):odrModel(odrModel),
                                                    handle(handle),
                                                    radius(radius),
                                                    squaredCurvatureWeight(squaredCurvatureWeight),
                                                    dataWeight(dataWeight)
            {}

            const ODRModel odrModel;
            const LinelSpaceHandle handle;
            const unsigned long radius;
            const double squaredCurvatureWeight;
            const double dataWeight;
        };
    }
}

#endif //LPMODEL_INITIALIZATIONLINEL_PARAMETERS_H
