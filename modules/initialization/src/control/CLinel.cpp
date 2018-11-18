#include "LPModel/initialization/control/CLinel.h"

using namespace LPModel::Initialization;

void CLinel::createLinelSet(LinelSet &lnlSet,
                            const Domain &domain,
                            const PixelMap &pixelMap)
{
    KSpace kspace;
    kspace.init(domain.lowerBound(),domain.upperBound(),true);

    struct _Linel
    {
        _Linel(Linel::LinelOrientation orientation): orientation(orientation),
                                              p1(-1),
                                              p2(-1){}

        int p1,p2;
        DGtal::Z2i::SCell linel;
        Linel::LinelOrientation orientation;
    };

    std::map<DGtal::Z2i::Point,_Linel> linelMap;
    typedef std::pair<DGtal::Z2i::Point,_Linel> Element;

    DGtal::Z2i::SCell pixelModel = kspace.sCell( DGtal::Z2i::Point(1,1),true);
    for(auto it1=pixelMap.begin();it1!=pixelMap.end();++it1)
    {
        if(it1->second.varIndex==-1) continue;
        DGtal::Z2i::Point p( it1->second.col, it1->second.row );
        DGtal::Z2i::SCells scells = kspace.sLowerIncident(kspace.sCell(p,pixelModel));

        for(auto it2=scells.begin();it2!=scells.end();++it2)
        {
            DGtal::Z2i::SCell linel = *it2;
            DGtal::Z2i::Point coord = linel.preCell().coordinates;

            if( linelMap.find(coord)==linelMap.end() )
            {
                if(coord(0)%2==0) //Vertical
                    linelMap.insert( Element(coord,_Linel(Linel::LinelOrientation::Up)) );
                else
                    linelMap.insert( Element(coord,_Linel(Linel::LinelOrientation::Right) ) );
            }
            _Linel& l = linelMap.at(coord);

            if(kspace.sSign(l.linel)) l.p1 = it1->second.varIndex;
            else l.p2 = it1->second.varIndex;
        }
    }

    int linelIndex=0;
    for(auto it=linelMap.begin();it!=linelMap.end();++it)
    {
        _Linel& l = it->second;
        lnlSet.insert( Linel( pixelMap.at(l.p1),pixelMap.at(l.p2),l.orientation,linelIndex++) );
    }

}