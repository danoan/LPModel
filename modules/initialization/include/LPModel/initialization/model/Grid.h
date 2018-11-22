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

            typedef Pixel::PixelMap PixelMap;
            typedef Edge::EdgeMap EdgeMap;
            typedef Linel::LinelMap LinelMap;

            Grid(const PixelMap& pixelMap,
                 const EdgeMap& edgeMap,
                 const LinelMap& linelMap):_pixelMap(pixelMap),
                                           _edgeMap(edgeMap),
                                           _linelMap(linelMap),
                                           pixelMap(_pixelMap),
                                           edgeMap(_edgeMap),
                                           linelMap(_linelMap)
            {}

            Grid(const DigitalSet& ds):pixelMap(_pixelMap),
                                       edgeMap(_edgeMap),
                                       linelMap(_linelMap)
            {
                CPixel::createPixelMap(_pixelMap,ds);
                CLinel::createLinelSet(_linelMap,ds.domain(),_pixelMap);
                CEdge::createEdgeMap(_edgeMap,_pixelMap,_linelMap);
            }

            const PixelMap& pixelMap;
            const EdgeMap& edgeMap;
            const LinelMap& linelMap;

        private:
            PixelMap _pixelMap;
            EdgeMap _edgeMap;
            LinelMap _linelMap;
        };
    }
}

#endif //LPMODEL_GRID_H
