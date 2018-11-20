#ifndef LPMODEL_PIXELINCIDENCE_H
#define LPMODEL_PIXELINCIDENCE_H

#include <LPModel/initialization/model/Pixel.h>

namespace LPModel
{
    namespace Constraints
    {
        namespace ClosedAndConnected
        {
            struct PixelIncidence
            {
                typedef Initialization::Pixel Pixel;

                PixelIncidence(const Pixel& pixel,
                               const bool posIncidence):pixel(pixel),
                                                        posIncidence(posIncidence)
                {}

                const Pixel& pixel;
                const bool posIncidence;
            };
        }
    }
}

#endif //LPMODEL_PIXELINCIDENCE_H
