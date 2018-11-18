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
            typedef std::map<int,Edge> EdgeMap;
            typedef std::pair<int,Edge> Element;

            typedef std::map<int,Pixel> PixelMap;
            typedef std::set<Linel> LinelSet;

            void createEdgeMap(EdgeMap& edgeMap,
                               const PixelMap& pixelMap,
                               const LinelSet& lnlSet);
        }
    }
}

#endif //LPMODEL_CEDGE_H
