#ifndef LPMODEL_INITIALIZATION_EDGE_H
#define LPMODEL_INITIALIZATION_EDGE_H

#include "Linel.h"

namespace LPModel
{
    namespace Initialization
    {
        struct Edge
        {
            enum EdgeOrientation{Left,Down,Right,Up};
            typedef std::map<int,Edge> EdgeMap;
            typedef std::pair<int,Edge> MapElement;

            Edge(const Linel l1,
                 const EdgeOrientation& orientation,
                 const unsigned long varIndex):l1(l1),
                                     orientation(orientation),
                                     varIndex(varIndex)
            {}

            const Linel l1;
            const EdgeOrientation orientation;
            const unsigned long varIndex;

            friend std::ostream& operator<<(std::ostream& os, const Edge& edge);
        };
    }
}

#endif //LPMODEL_INITIALIZATION_EDGE_H
