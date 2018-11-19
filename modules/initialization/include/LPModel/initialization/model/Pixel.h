#ifndef LPMODEL_PIXEL_H
#define LPMODEL_PIXEL_H

#include "DGtal/helpers/StdDefs.h"

namespace LPModel
{
    namespace Initialization
    {
        struct Pixel
        {
            enum CellOrientation{Clockwise,Counterclockwise};

            typedef DGtal::Z2i::Point KPoint;

            typedef std::map<KPoint, Pixel> PixelMap;
            typedef std::pair<KPoint, Pixel> MapElement;

            Pixel(int row, int col,unsigned int varIndex):row(row),
                                                 col(col),
                                                 varIndex(varIndex),
                                                 orientation(CellOrientation::Counterclockwise)
            {}

            const int row,col;
            const CellOrientation orientation;
            const unsigned int varIndex;
        };
    }
}

#endif //LPMODEL_PIXEL_H
