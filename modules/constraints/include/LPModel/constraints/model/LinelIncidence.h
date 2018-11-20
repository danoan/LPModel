#ifndef LPMODEL_LINELINCIDENCE_H
#define LPMODEL_LINELINCIDENCE_H

#include "PixelIncidence.h"
#include "EdgeIncidence.h"

namespace LPModel
{
    namespace Constraints
    {
        namespace ClosedAndConnected
        {
            struct LinelIncidence
            {
                LinelIncidence(const PixelIncidence& pixel1,
                               const PixelIncidence& pixel2,
                               const EdgeIncidence& edge1,
                               const EdgeIncidence& edge2):pixel1(pixel1),
                                                           pixel2(pixel2),
                                                           edge1(edge1),
                                                           edge2(edge2)
                {}

                const PixelIncidence pixel1;
                const PixelIncidence pixel2;

                const EdgeIncidence edge1;
                const EdgeIncidence edge2;
            };
        }
    }
}

#endif //LPMODEL_LINELINCIDENCE_H
