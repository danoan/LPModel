#ifndef LPMODEL_INITIALIZATIONLINEL_PIXEL_H
#define LPMODEL_INITIALIZATIONLINEL_PIXEL_H

#include "DGtal/helpers/StdDefs.h"
#include "IVariable.h"

namespace LPModel
{
    namespace Initialization
    {
        struct Pixel:public IVariable
        {
            enum CellOrientation{Clockwise,Counterclockwise};
            enum CellType{Variable,AuxiliarBkg,AuxiliarFrg,AuxiliarInvalid};

            typedef DGtal::Z2i::Point KPoint;

            typedef std::map<KPoint, Pixel> PixelMap;
            typedef std::pair<KPoint, Pixel> MapElement;

            Pixel(int row,
                  int col,
                  unsigned long varIndex,
                  CellType ct):IVariable(IVariable::Pixel,
                                         varIndex),
                               row(row),
                               col(col),
                               orientation(CellOrientation::Counterclockwise),
                               ct(ct)
            {}

            const int row,col;
            const CellOrientation orientation;
            const CellType ct;

            friend std::ostream& operator<<(std::ostream& os, const Pixel& pixel);

        };
    }
}


#endif //LPMODEL_INITIALIZATIONLINEL_PIXEL_H
