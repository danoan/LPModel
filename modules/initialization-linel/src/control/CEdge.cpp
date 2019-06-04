#include "LPModel/initialization/control/CEdge.h"

namespace LPModel{ namespace Initialization{

    std::ostream& operator<<(std::ostream& os, const Edge& edge)
    {
        os.write( (char*) &edge.l1.linelIndex, sizeof(edge.l1.linelIndex));
        os.write( (char*) &edge.orientation, sizeof(edge.orientation));
        os.write( (char*) &edge.varIndex, sizeof(edge.varIndex));

        return os;
    }
}}

using namespace LPModel::Initialization;

bool CEdge::isSequence(const int firstEdgeVar,
                       const EdgeMap& edgeMap)
{
    std::vector<int> seq;
    for(auto it=edgeMap.begin();it!=edgeMap.end();++it)
    {
        seq.push_back( it->second.varIndex - firstEdgeVar );
    }

    std::sort(seq.begin(),seq.end());
    int n=0;
    for(auto it=seq.begin();it!=seq.end();++it,++n)
    {
        if( (*it)!=n ) return false;
    }

    return true;
}

void CEdge::createEdgeMap(EdgeMap &edgeMap,
                          const PixelMap &pixelMap,
                          const LinelMap &linelMap)
{
    LinelMap::const_iterator it = linelMap.begin();

    int varIndex;
    int firstEdgeVar = pixelMap.size()-3;
    do
    {
        const Linel& linel = it->second;

        varIndex =  CLinel::edgeBaseIndex(firstEdgeVar,linel.linelIndex); //TODO: was CLinel::edgeBaseIndex(firstLinelVar,firstEdgeVar,linel.linelIndex);

        if(linel.orientation==Linel::LinelOrientation::Up
           || linel.orientation==Linel::LinelOrientation::Down)
        {
        edgeMap.insert( Edge::MapElement(varIndex,Edge(linel,Edge::EdgeOrientation::Up,varIndex) ));
        edgeMap.insert( Edge::MapElement(varIndex+1,Edge(linel,Edge::EdgeOrientation::Down,varIndex+1)));
        }
        else
        {
        edgeMap.insert( Edge::MapElement(varIndex,Edge(linel,Edge::EdgeOrientation::Right,varIndex)));
        edgeMap.insert( Edge::MapElement(varIndex+1,Edge(linel,Edge::EdgeOrientation::Left,varIndex+1)));
        }

        ++it;

    }while(it!=linelMap.end());


    assert(isSequence(firstEdgeVar,edgeMap));
}

Edge CEdge::readEdge(std::istream& is, const LinelIndexMap& lim)
{
    unsigned long linelIndex, edgeIndex;
    int orientation;

    is.read( (char*) &linelIndex, sizeof(linelIndex));
    is.read( (char*) &orientation, sizeof(orientation));
    is.read( (char*) &edgeIndex, sizeof(edgeIndex) );

    return Edge(lim.at(linelIndex),
                (Edge::EdgeOrientation) orientation,
                edgeIndex);

}