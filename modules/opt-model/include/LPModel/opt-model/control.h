#ifndef LPMODEL_CONTROL_H
#define LPMODEL_CONTROL_H

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/geometry/volumes/distance/DistanceTransformation.h>

#include <DIPaCUS/components/SetOperations.h>
#include <DIPaCUS/derivates/Misc.h>

#include "OptModel.h"

namespace LPModel
{
    namespace OptModel
    {
        typedef DGtal::Z2i::DigitalSet DigitalSet;
        typedef DGtal::Z2i::Domain Domain;
        typedef DGtal::Z2i::KSpace KSpace;
        typedef DGtal::Z2i::Point Point;
        typedef DGtal::Z2i::SCell SCell;

        typedef DGtal::DistanceTransformation<DGtal::Z2i::Space, DigitalSet, DGtal::Z2i::L2Metric> DTL2;

        DigitalSet level(const DTL2& dtL2, int lessThan, int greaterThan);
        OptModel createOptModel(const DigitalSet& original, int optWidth);

    }
}

#endif //LPMODEL_CONTROL_H
