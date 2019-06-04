#ifndef LPMODEL_INITIALIZATIONLINEL_CPIXEL_H
#define LPMODEL_INITIALIZATIONLINEL_CPIXEL_H

#include "DGtal/helpers/StdDefs.h"

#include "LPModel/initialization/model/Pixel.h"

namespace LPModel
{
    namespace Initialization
    {
        namespace CPixel
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::KSpace KSpace;
            typedef DGtal::Z2i::Domain Domain;

            typedef Pixel::PixelMap PixelMap;
            typedef Pixel::MapElement MapElement;

            void createPixelMap(PixelMap &pxlMap,
                                const DigitalSet &ds);

            Pixel readPixel(std::istream& is);
        }
    }
}

#endif //LPMODEL_INITIALIZATION_CPIXEL_H
