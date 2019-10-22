#include <DGtal/io/boards/Board2D.h>
#include "LPModel/initialization/control/CLinel.h"

namespace LPModel{ namespace Initialization{
    std::ostream& operator<<(std::ostream& os, const Linel& linel)
    {
        os.write( (char*) &linel.x, sizeof(linel.x) );
        os.write( (char*) &linel.y, sizeof(linel.y) );
        os.write( (char*) &linel.p1.varIndex, sizeof(linel.p1.varIndex) );
        os.write( (char*) &linel.p2.varIndex, sizeof(linel.p2.varIndex) );
        os.write( (char*) &linel.orientation ,sizeof(linel.orientation) );
        os.write( (char*) &linel.linelIndex, sizeof(linel.linelIndex) );

        return os;
    }
}}

using namespace LPModel::Initialization;

CLinel::Internal::IncidentLinels CLinel::Internal::incidentLinels(const KPoint &pixel)
{
    return
    IncidentLinels ( { SignedKPoint( pixel+KPoint(1,0),false ),
                       SignedKPoint( pixel+KPoint(0,1),true ),
                       SignedKPoint( pixel+KPoint(-1,0),true ),
                       SignedKPoint( pixel+KPoint(0,-1), false) } );
}


CLinel::Internal::IncidentPixels CLinel::Internal::incidentPixels(const _Linel &linel)
{
    if(linel.orientation==Linel::LinelOrientation::Up || linel.orientation==Linel::LinelOrientation::Down)
    {
        return
                IncidentPixels ( { linel.linelCoord + KPoint(-1,0) ,
                                   linel.linelCoord + KPoint(1,0) } );
    } else
    {
        return
                IncidentPixels ( { linel.linelCoord +KPoint(0,-1) ,
                                   linel.linelCoord + KPoint(0,1) } );
    }
}

CLinel::KPoint CLinel::Internal::findAuxiliarPixelCoord(const PixelMap& pixelMap,
                                                        Pixel::CellType ct)
{
    for(auto it=pixelMap.begin();it!=pixelMap.end();++it)
    {
        if(it->second.ct==ct) return it->first;
    }
}

void CLinel::Internal::auxiliaryMap(AuxLinelMap& auxLinelMap,
                                    const PixelMap& pixelMap)
{
    KPoint auxInvalidPixelCoord  = findAuxiliarPixelCoord(pixelMap,Pixel::CellType::AuxiliarInvalid);

    std::set<KPoint> testSet;
    int j=0;
    for(auto it1=pixelMap.begin();it1!=pixelMap.end();++it1)
    {
        if(it1->second.ct!=Pixel::CellType::Variable) continue;
        DGtal::Z2i::Point pCoord = it1->first;
        Internal::IncidentLinels incLinels = Internal::incidentLinels(pCoord);

        for(auto it2=incLinels.begin();it2!=incLinels.end();++it2)
        {
            KPoint coord= it2->coord;

            if( auxLinelMap.find(coord)==auxLinelMap.end() )
            {
                if(coord(0)%2==0) //Vertical
                    auxLinelMap.insert( AuxMapElement(coord,_Linel(coord,auxInvalidPixelCoord ,Linel::LinelOrientation::Up)) );
                else
                    auxLinelMap.insert( AuxMapElement(coord,_Linel(coord,auxInvalidPixelCoord  ,Linel::LinelOrientation::Right) ) );
            }
            _Linel& l = auxLinelMap.at(coord);

            if(it2->pos) l.pCoord1 = pCoord;
            else l.pCoord2 = pCoord;
        }

    }
}

void CLinel::Internal::fixInvalidAuxPixels(AuxLinelMap& auxLinelMap,
                                           const PixelMap& pixelMap,
                                           const DigitalSet& optRegion,
                                           const DigitalSet& trustFrg)
{
    KPoint auxInvalidPixelCoord  = findAuxiliarPixelCoord(pixelMap,Pixel::CellType::AuxiliarInvalid);
    KPoint auxBkgPixelCoord  = findAuxiliarPixelCoord(pixelMap,Pixel::CellType::AuxiliarBkg);
    KPoint auxFrgPixelCoord  = findAuxiliarPixelCoord(pixelMap,Pixel::CellType::AuxiliarFrg);

    for(auto it=auxLinelMap.begin();it!=auxLinelMap.end();++it)
    {
        IncidentPixels pixels = incidentPixels( it->second );
        for(auto itp=pixels.begin();itp!=pixels.end();++itp)
        {

            if( trustFrg.operator()( *itp ) )
            {
                if(it->second.pCoord1==auxInvalidPixelCoord)
                {
                    it->second.pCoord1 = auxFrgPixelCoord;
                }
                if(it->second.pCoord2==auxInvalidPixelCoord)
                {
                    it->second.pCoord2 = auxFrgPixelCoord;
                }

            }
            else if( !optRegion.operator()( *itp ) )
            {
                if(it->second.pCoord1==auxInvalidPixelCoord) it->second.pCoord1 = auxBkgPixelCoord;
                if(it->second.pCoord2==auxInvalidPixelCoord) it->second.pCoord2 = auxBkgPixelCoord;
            }

        }



    }
}

bool CLinel::Internal::validAuxLinelMap(const AuxLinelMap &auxLinelMap,
                                        const PixelMap& pixelMap)
{
    KPoint auxInvalidPixelCoord  = findAuxiliarPixelCoord(pixelMap,Pixel::CellType::AuxiliarInvalid);
    for(auto it=auxLinelMap.begin();it!=auxLinelMap.end();++it)
    {
        if( it->second.pCoord1 == auxInvalidPixelCoord )
        {
            return false;
        }
        if( it->second.pCoord2 == auxInvalidPixelCoord )
        {
            return false;
        }
    }

    return true;
}

int CLinel::edgeBaseIndex(const int firstEdgeVar,
                          const int linelIndex)
{
    return firstEdgeVar + (linelIndex*2);
}


void CLinel::createLinelSet(LinelMap &linelMap,
                            const DigitalSet& trustFrg,
                            const DigitalSet& optRegion,
                            const PixelMap &pixelMap)
{

    Internal::AuxLinelMap auxLinelMap;
    Internal::auxiliaryMap(auxLinelMap,pixelMap);

    Internal::fixInvalidAuxPixels(auxLinelMap,pixelMap,optRegion,trustFrg);
    assert( Internal::validAuxLinelMap(auxLinelMap,pixelMap) );


    unsigned long linelIndex=0;
    for(auto it=auxLinelMap.begin();it!=auxLinelMap.end();++it)
    {
        Internal::_Linel& l = it->second;
        linelMap.insert( Linel::MapElement(l.linelCoord, Linel( l.linelCoord(0),
                                                                l.linelCoord(1),
                                                                pixelMap.at(l.pCoord1),
                                                                pixelMap.at(l.pCoord2),
                                                                l.orientation,
                                                                linelIndex++) ) );
    }

}

Linel CLinel::readLinel(std::istream& is, const PixelIndexMap& pim)
{
    int x,y;
    unsigned long p1VarIndex,p2VarIndex,linelIndex;
    int orientation;

    is.read( (char*) &x, sizeof(x));
    is.read( (char*) &y, sizeof(y));
    is.read( (char*) &p1VarIndex, sizeof(p1VarIndex));
    is.read( (char*) &p2VarIndex, sizeof(p2VarIndex));
    is.read( (char*) &orientation, sizeof(orientation));
    is.read( (char*) &linelIndex, sizeof(linelIndex));

    return Linel(x,
                 y,
                 pim.at(p1VarIndex),
                 pim.at(p2VarIndex),
                 (Linel::LinelOrientation) orientation,
                 linelIndex);

}