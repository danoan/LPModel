#ifndef LPMODEL_TERMSLINEL_DATA_API_H
#define LPMODEL_TERMSLINEL_DATA_API_H

#include <LPModel/initialization/model/Grid.h>
#include <LPModel/initialization/model/Parameters.h>
#include <LPModel/terms/model/Term.h>

namespace LPModel
{
    namespace Terms
    {
        namespace DataFidelity
        {
            namespace API
            {
                typedef Initialization::Parameters Parameters;
                typedef Initialization::Grid Grid;

                typedef DGtal::Z2i::Point Point;

                Term prepare(const Parameters& prm,
                             const Grid& grid,
                             double weight);

                inline Point fromInterpixelToPixelSpace(const Point& ipPoint){ return (ipPoint + Point(1,1))/2.0;}
                inline Point fromPixelToInterpixelSpace(const Point& pPoint){ return pPoint*2 - Point(1,1);}
            }
        }
    }
}

#endif //LPMODEL_TERMSLINEL_DATA_API_H
