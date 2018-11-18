#ifndef LPMODEL_API_H
#define LPMODEL_API_H

#include "SCaBOliC/Core/ODRModel.h"
#include "SCaBOliC/Core/ODRPixels.h"
#include "DIPaCUS/derivates/Misc.h"
#include "DIPaCUS/base/Representation.h"

#include "model/Grid.h"
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

            typedef DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::FourNeighborhoodPredicate> FourDigitalBoundary;
            typedef SCaBOliC::Core::ODRPixels ODRPixels;
            typedef SCaBOliC::Core::ODRModel ODRModel;

            ODRModel createOdrModel(const DigitalSet& originalDS);
            Grid createGrid(const DigitalSet& ds);

            namespace Internal
            {
                int uniqueLinels(const DigitalSet& digitalBoundary);
            }
        }
    }
}

#endif //LPMODEL_API_H
