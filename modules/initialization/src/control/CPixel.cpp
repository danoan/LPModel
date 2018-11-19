#include "LPModel/initialization/control/CPixel.h"

using namespace LPModel::Initialization;

void CPixel::createPixelMap(PixelMap& pxlMap,
                            const DigitalSet& ds)
{

    int varIndex=0;
    Pixel::KPoint lb,ub;
    ds.computeBoundingBox(lb,ub);

    Pixel::KPoint outRange(ub+Pixel::KPoint(1,1));
    pxlMap.insert( MapElement( outRange ,Pixel(outRange(1),outRange(0),-1) ) );
    for(auto it=ds.begin();it!=ds.end();++it)
    {
        pxlMap.insert( MapElement(*it,Pixel( (*it)(1),(*it)(0), varIndex ) ) );
        ++varIndex;
    }
}