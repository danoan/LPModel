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
    KSpace kspace;

    const Domain& domain = ds.domain();
    kspace.init(domain.lowerBound(),domain.upperBound(),true);

    Pixel::KPoint ub = kspace.upperCell().preCell().coordinates;

    Pixel::KPoint auxBkgCoord(ub+Pixel::KPoint(3,3));
    Pixel::KPoint auxFkgCoord(ub+Pixel::KPoint(5,5));
    Pixel::KPoint auxInvalidCoord(ub+Pixel::KPoint(7,7));

    pxlMap.insert( MapElement( auxInvalidCoord,
                               Pixel(auxInvalidCoord(1),auxInvalidCoord(0),-1,Pixel::CellType::AuxiliarInvalid) ) );

    pxlMap.insert( MapElement( auxBkgCoord,
                               Pixel(auxBkgCoord(1),auxBkgCoord(0),-2,Pixel::CellType::AuxiliarBkg) ) );

    pxlMap.insert( MapElement( auxFkgCoord,
                               Pixel(auxFkgCoord(1),auxFkgCoord(0),-3,Pixel::CellType::AuxiliarFrg) ) );

    KSpace::SCell pixelModel = kspace.sCell(KSpace::Point(1,1),true);
    for(auto it=ds.begin();it!=ds.end();++it)
    {
        KSpace::Point kCoords = kspace.sKCoords( kspace.sCell(*it,pixelModel) );

        pxlMap.insert( MapElement(kCoords,Pixel( kCoords[1],kCoords[0], varIndex, Pixel::CellType::Variable) ) );
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