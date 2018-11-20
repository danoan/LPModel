#ifndef LPMODEL_EDGEINCIDENCE_H
#define LPMODEL_EDGEINCIDENCE_H

#include <LPModel/initialization/model/Edge.h>

namespace LPModel
{
    namespace Constraints
    {
        namespace ClosedAndConnected
        {
            struct EdgeIncidence
            {
                typedef Initialization::Edge  Edge;

                EdgeIncidence(const Edge& edge,
                              const bool posIncidence):edge(edge),
                                                       posIncidence(posIncidence)
                {}

                const Edge& edge;
                const bool posIncidence;
            };
        }
    }
}

#endif //LPMODEL_EDGEINCIDENCE_H
