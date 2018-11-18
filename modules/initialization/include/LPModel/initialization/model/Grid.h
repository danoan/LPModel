#ifndef LPMODEL_GRID_H
#define LPMODEL_GRID_H

#include <LPModel/initialization/control/CPixel.h>
#include <LPModel/initialization/control/CLinel.h>
#include <LPModel/initialization/control/CEdge.h>
#include "Pixel.h"
#include "Edge.h"
#include "Linel.h"

namespace LPModel
{
    namespace Initialization
    {
        struct Grid
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef std::map<int, Pixel> PixelMap;
            typedef std::map<int,Edge> EdgeMap;
            typedef std::set<Linel> LinelSet;

            Grid(const DigitalSet& ds):pixelMap(_pixelMap),
                                       edgeMap(_edgeMap),
                                       linelSet(_linelSet)
            {
                CPixel::createPixelMap(_pixelMap,ds);
                CLinel::createLinelSet(_linelSet,ds.domain(),_pixelMap);
                CEdge::createEdgeMap(_edgeMap,_pixelMap,_linelSet);
            }

            const PixelMap& pixelMap;
            const EdgeMap& edgeMap;
            const LinelSet& linelSet;

        private:
            PixelMap _pixelMap;
            EdgeMap _edgeMap;
            LinelSet _linelSet;
        };
    }
}

#endif //LPMODEL_GRID_H
