#include "LPModel/initialization/control/CPixel.h"

namespace LPModel{ namespace Initialization{
    std::ostream& operator<<(std::ostream& os, const Pixel& pixel)
    {
        os.write( (char*) &pixel.row, sizeof(pixel.row));
        os.write( (char*) &pixel.col, sizeof(pixel.col));
        os.write( (char*) &pixel.orientation, sizeof(pixel.orientation));
        os.write( (char*) &pixel.varIndex, sizeof(pixel.varIndex));
        os.write( (char*) &pixel.ct, sizeof(pixel.ct));

        return os;
    }
}}

using namespace LPModel::Initialization;

void CPixel::createPixelMap(PixelMap& pxlMap,
                            const DigitalSet& ds)
{

    int varIndex=0;
    Pixel::KPoint lb,ub;
    ds.computeBoundingBox(lb,ub);

    Pixel::KPoint outRange(ub+Pixel::KPoint(1,1));
    pxlMap.insert( MapElement( outRange ,
                               Pixel(outRange(1),outRange(0),-1,Pixel::CellType::Auxiliar) ) );
    for(auto it=ds.begin();it!=ds.end();++it)
    {
        pxlMap.insert( MapElement(*it,Pixel( (*it)(1),(*it)(0), varIndex, Pixel::CellType::Variable) ) );
        ++varIndex;
    }
}

Pixel CPixel::readPixel(std::istream& is)
{
    int row,col;
    Pixel::CellOrientation orientation;
    unsigned long varIndex;
    Pixel::CellType ct;

    is.read( (char*) &row,sizeof(row));
    is.read( (char*) &col,sizeof(col));
    is.read( (char*) &orientation,sizeof(orientation));
    is.read( (char*) &varIndex,sizeof(varIndex));
    is.read( (char*) &ct,sizeof(ct));

    return Pixel(row,col,varIndex,ct);
}