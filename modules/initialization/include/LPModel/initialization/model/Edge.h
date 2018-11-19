#ifndef LPMODEL_EDGE_H
#define LPMODEL_EDGE_H

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

            Edge(const Linel& l1,
                 const EdgeOrientation& orientation,
                 const int varIndex):l1(l1),
                                     orientation(orientation),
                                     varIndex(varIndex)
            {}

            const Linel &l1;
            const EdgeOrientation orientation;
            const int varIndex;
        };
    }
}

#endif //LPMODEL_EDGE_H
