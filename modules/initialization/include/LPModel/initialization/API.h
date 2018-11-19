#ifndef LPMODEL_API_H
#define LPMODEL_API_H

#include "SCaBOliC/Core/ODRModel.h"
#include "SCaBOliC/Core/ODRInterpixels.h"
#include "DIPaCUS/derivates/Misc.h"
#include "DIPaCUS/base/Representation.h"

#include "model/Grid.h"
#include "model/Parameters.h"
#include "control/CPixel.h"
#include "control/CEdge.h"
#include "control/CLinel.h"

namespace LPModel
{
    namespace Initialization
    {
        namespace API
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;
            typedef DigitalSet::Point Point;
            typedef Point KPoint;

            typedef DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate> FourDigitalBoundary;
            typedef SCaBOliC::Core::ODRInterpixels ODRInterpixels;
            typedef SCaBOliC::Core::ODRModel ODRModel;

            Parameters initParameters(const DigitalSet &originalDS);
            Grid createGrid(const DigitalSet& ds);

            namespace Internal
            {
                int boundaryLinels(const DigitalSet& ds);
                DigitalSet pixelOptRegion(const DigitalSet& ds);
            }
        }
    }
}

#endif //LPMODEL_API_H
