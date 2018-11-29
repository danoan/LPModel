#ifndef LPMODEL_INITIALIZATION_CEDGE_H
#define LPMODEL_INITIALIZATION_CEDGE_H

#include <set>
#include <map>

#include "LPModel/initialization/model/Edge.h"
#include "LPModel/initialization/model/Linel.h"
#include "LPModel/initialization/model/Pixel.h"

#include <LPModel/initialization/control/CLinel.h>

namespace LPModel
{
    namespace Initialization
    {
        namespace CEdge
        {
            typedef Edge::EdgeMap EdgeMap;

            typedef Pixel::PixelMap PixelMap;
            typedef Linel::LinelMap LinelMap;

            typedef std::map<unsigned long,Linel> LinelIndexMap;

            void createEdgeMap(EdgeMap& edgeMap,
                               const PixelMap& pixelMap,
                               const LinelMap& linelMap);

            int edgeBaseIndex(const int firstLinelVar,
                              const int firstEdgeVar,
                              const int linelIndex);

            Edge readEdge(std::istream& is, const LinelIndexMap& lim);

            bool isSequence(const int firstEdgeVar,
                            const EdgeMap& edgeMap);
        }
    }
}

#endif //LPMODEL_INITIALIZATION_CEDGE_H
