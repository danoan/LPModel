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
            typedef DGtal::AccFlower2D<Space> Flower;

            DigitalSet square();
            DigitalSet flower();

            namespace Internal
            {
                template<typename TShape>
                DigitalSet shapeDigitizer(TShape shape)
                {
                    DGtal::GaussDigitizer<Space,TShape> gd;

                    gd.init(shape.getLowerBound(),shape.getUpperBound(),1.0);
                    gd.attach(shape);

                    Domain domain(shape.getLowerBound(),shape.getUpperBound());
                    DigitalSet ds(domain);
                    DGtal::Shapes<Domain>::digitalShaper(ds,gd);

                    return ds;
                }
            }
        }
    }
}

#endif //LPMODEL_SHAPES_H
