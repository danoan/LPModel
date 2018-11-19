#ifndef LPMODEL_CEDGE_H
#define LPMODEL_CEDGE_H

#include <set>
#include <map>

#include "LPModel/initialization/model/Edge.h"
#include "LPModel/initialization/model/Linel.h"
#include "LPModel/initialization/model/Pixel.h"

namespace LPModel
{
    namespace Initialization
    {
        namespace CEdge
        {
            typedef Edge::EdgeMap EdgeMap;

            typedef Pixel::PixelMap PixelMap;
            typedef Linel::LinelMap LinelMap;

            void createEdgeMap(EdgeMap& edgeMap,
                               const PixelMap& pixelMap,
                               const LinelMap& linelMap);
        }
    }
}

#endif //LPMODEL_CEDGE_H
