#ifndef LPMODEL_SHAPES_H
#define LPMODEL_SHAPES_H

#include "DGtal/helpers/StdDefs.h"
#include "DGtal/shapes/ShapeFactory.h"
#include "DGtal/shapes/GaussDigitizer.h"
#include "DGtal/shapes/Shapes.h"

#include "DIPaCUS/components/Transform.h"

namespace LPModel
{
    namespace Initialization
    {
        namespace Shapes
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;

            typedef DGtal::Z2i::Space Space;
            typedef DGtal::NGon2D<Space> NGon2D;
            typedef DGtal::GaussDigitizer<Space,NGon2D> GaussDigitizer;

            DigitalSet square();
        }
    }
}

#endif //LPMODEL_SHAPES_H
