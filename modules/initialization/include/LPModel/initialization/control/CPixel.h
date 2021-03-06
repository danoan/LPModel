#ifndef LPMODEL_INITIALIZATION_CPIXEL_H
#define LPMODEL_INITIALIZATION_CPIXEL_H

#include "DGtal/helpers/StdDefs.h"

#include "../model/Pixel.h"

namespace LPModel
{
    namespace Initialization
    {
        namespace CPixel
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef Pixel::PixelMap PixelMap;
            typedef Pixel::MapElement MapElement;

            void createPixelMap(PixelMap &pxlMap,
                                const DigitalSet &ds);

            Pixel readPixel(std::istream& is);
        }
    }
}

#endif //LPMODEL_INITIALIZATION_CPIXEL_H
