#include "LPModel/initialization/control/CEdge.h"

using namespace LPModel::Initialization;

void CEdge::createEdgeMap(EdgeMap &edgeMap,
                          const PixelMap &pixelMap,
                          const LinelMap &linelMap)
{
    LinelMap::const_iterator it = linelMap.begin();
    int varIndex = pixelMap.size();
    do
    {
    const Linel& linel = it->second;

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

    varIndex+=2;
    ++it;

    }while(it!=linelMap.end());
}