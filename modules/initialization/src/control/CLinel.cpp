#include "LPModel/initialization/control/CLinel.h"

using namespace LPModel::Initialization;

CLinel::Internal::IncidentLinels CLinel::Internal::incidentLinels(const KPoint &pixel)
{
    return
    IncidentLinels ( { SignedKPoint( pixel+KPoint(1,0),true ),
                       SignedKPoint( pixel+KPoint(0,1),false ),
                       SignedKPoint( pixel+KPoint(-1,0),false ),
                       SignedKPoint( pixel+KPoint(0,-1), true) } );
}

CLinel::KPoint CLinel::Internal::findOutRangePixelCoord(const PixelMap& pixelMap)
{
    KPoint outRangePixelCoord;
    for(auto it=pixelMap.begin();it!=pixelMap.end();++it)
    {
        if(it->second.varIndex==-1) return it->first;
    }
}

void CLinel::Internal::auxiliaryMap(AuxLinelMap& auxLinelMap,
                                    const Domain& domain,
                                    const PixelMap& pixelMap)
{
    KSpace kspace;
    kspace.init(domain.lowerBound(),domain.upperBound(),true);

    KPoint outRangePixelCoord = findOutRangePixelCoord(pixelMap);
    DGtal::Z2i::SCell pixelModel = kspace.sCell( DGtal::Z2i::Point(1,1),true);

    std::set<KPoint> testSet;
    int j=0;
    for(auto it1=pixelMap.begin();it1!=pixelMap.end();++it1)
    {
        if(it1->second.varIndex==-1) continue;
        DGtal::Z2i::Point pCoord = it1->first;
        Internal::IncidentLinels incLinels = Internal::incidentLinels(pCoord);

        for(auto it2=incLinels.begin();it2!=incLinels.end();++it2)
        {
            KPoint coord= it2->coord;

            if( auxLinelMap.find(coord)==auxLinelMap.end() )
            {
                if(coord(0)%2==0) //Vertical
                    auxLinelMap.insert( AuxMapElement(coord,_Linel(coord,outRangePixelCoord,Linel::LinelOrientation::Up)) );
                else
                    auxLinelMap.insert( AuxMapElement(coord,_Linel(coord,outRangePixelCoord,Linel::LinelOrientation::Right) ) );
            }
            _Linel& l = auxLinelMap.at(coord);

            if(it2->pos) l.pCoord1 = pCoord;
            else l.pCoord2 = pCoord;
        }

    }
}

int CLinel::edgeBaseIndex(const int firstLinelVar,
                          const int firstEdgeVar,
                          const int linelIndex)
{
    int linelNum = (linelIndex - firstLinelVar);
    return firstEdgeVar + (linelNum*2);
}


void CLinel::createLinelSet(LinelMap &linelMap,
                            const Domain &domain,
                            const PixelMap &pixelMap)
{

    Internal::AuxLinelMap auxLinelMap;
    Internal::auxiliaryMap(auxLinelMap,domain,pixelMap);


    unsigned long linelIndex=pixelMap.size();
    for(auto it=auxLinelMap.begin();it!=auxLinelMap.end();++it)
    {
        Internal::_Linel& l = it->second;
        linelMap.insert( Linel::MapElement(l.linelCoord, Linel( pixelMap.at(l.pCoord1),
                                                                pixelMap.at(l.pCoord2),
                                                                l.orientation,
                                                                linelIndex++) ) );
    }

}