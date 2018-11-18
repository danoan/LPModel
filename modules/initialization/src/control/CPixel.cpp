#include "LPModel/initialization/control/CPixel.h"

using namespace LPModel::Initialization;

void CPixel::createPixelMap(PixelMap& pxlMap,
                            const DigitalSet& ds)
{

    int varIndex=0;

    pxlMap.insert( Element(-1,Pixel(0,0,-1)));
    for(auto it=ds.begin();it!=ds.end();++it)
    {
        pxlMap.insert( Element(varIndex,Pixel( (*it)(1),(*it)(0), varIndex ) ) );
        ++varIndex;
    }
}