#ifndef LPMODEL_PIXEL_H
#define LPMODEL_PIXEL_H

namespace LPModel
{
    namespace Initialization
    {
        struct Pixel
        {
            enum CellOrientation{Clockwise,Counterclockwise};

            Pixel(int row, int col,int varIndex):row(row),
                                                 col(col),
                                                 varIndex(varIndex),
                                                 orientation(CellOrientation::Counterclockwise)
            {}

            const int row,col;
            const CellOrientation orientation;
            const int varIndex;
        };
    }
}

#endif //LPMODEL_PIXEL_H
