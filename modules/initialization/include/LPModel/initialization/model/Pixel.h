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
            enum CellType{Variable,Auxiliar};

            typedef DGtal::Z2i::Point KPoint;

            typedef std::map<KPoint, Pixel> PixelMap;
            typedef std::pair<KPoint, Pixel> MapElement;

            Pixel(int row,
                  int col,
                  unsigned long varIndex,
                  CellType ct):row(row),
                               col(col),
                               varIndex(varIndex),
                               orientation(CellOrientation::Counterclockwise),
                               ct(ct)
            {}

            const int row,col;
            const CellOrientation orientation;
            const unsigned long varIndex;
            const CellType ct;

            friend std::ostream& operator<<(std::ostream& os, const Pixel& pixel);

        };
    }
}


#endif //LPMODEL_PIXEL_H
