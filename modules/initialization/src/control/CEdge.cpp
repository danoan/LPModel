#include "LPModel/initialization/control/CEdge.h"

using namespace LPModel::Initialization;

void CEdge::createEdgeMap(EdgeMap &edgeMap,
                          const PixelMap &pixelMap,
                          const LinelSet &lnlSet)
{
    LinelSet::const_iterator it = lnlSet.begin();
    int varIndex = pixelMap.size();
    do
    {
    const Linel& linel = *it;

    if(linel.orientation==Linel::LinelOrientation::Up
       || linel.orientation==Linel::LinelOrientation::Down)
    {
    edgeMap.insert( Element(varIndex,Edge(linel,Edge::EdgeOrientation::Up,varIndex) ));
    edgeMap.insert( Element(varIndex+1,Edge(linel,Edge::EdgeOrientation::Down,varIndex+1)));
    }
    else
    {
    edgeMap.insert(Element(varIndex,Edge(linel,Edge::EdgeOrientation::Right,varIndex)));
    edgeMap.insert(Element(varIndex+1,Edge(linel,Edge::EdgeOrientation::Left,varIndex+1)));
    }

    varIndex+=2;
    ++it;

    }while(it!=lnlSet.end());
}