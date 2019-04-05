#ifndef LPMODEL_INITIALIZATION_EDGE_H
#define LPMODEL_INITIALIZATION_EDGE_H

#include "Linel.h"
#include "IVariable.h"

namespace LPModel
{
    namespace Initialization
    {
        struct Edge:public IVariable
        {
            enum EdgeOrientation{Left,Down,Right,Up};
            typedef std::map<int,Edge> EdgeMap;
            typedef std::pair<int,Edge> MapElement;

            Edge(const Linel l1,
                 const EdgeOrientation& orientation,
                 const unsigned long varIndex):IVariable(IVariable::Edge,varIndex),
                                               l1(l1),
                                               orientation(orientation)

            {}

            const Linel l1;
            const EdgeOrientation orientation;

            friend std::ostream& operator<<(std::ostream& os, const Edge& edge);
        };
    }
}

#endif //LPMODEL_INITIALIZATION_EDGE_H
