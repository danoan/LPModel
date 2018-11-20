#include "LPModel/constraints/ClosedAndConnected.h"

using namespace LPModel::Constraints;

bool ClosedAndConnected::Internal::positiveIncident(const Linel &linel,
                                                    const Pixel &pixel)
{
    bool r;
    if(linel.orientation==Linel::LinelOrientation::Down ||
       linel.orientation==Linel::LinelOrientation::Left) r=true;
    else r=false;

    if(pixel.orientation==Pixel::CellOrientation::Clockwise) r=!r;

    return r;
}

bool ClosedAndConnected::Internal::positiveIncident(const Linel &linel,
                                                    const Edge &edge)
{
    if(linel.orientation==Linel::LinelOrientation::Up ||
       linel.orientation==Linel::LinelOrientation::Down)
    {
        assert(edge.orientation==Edge::EdgeOrientation::Up ||
               edge.orientation==Edge::EdgeOrientation::Down);
    }
    else
    {
        assert(edge.orientation==Edge::EdgeOrientation::Left ||
               edge.orientation==Edge::EdgeOrientation::Right);
    }

    return edge.orientation==linel.orientation;

}

void ClosedAndConnected::Internal::pixelIncident(PartialPixelIncidence& ppi,
                                                 const Grid &grid)
{
    for(auto it=grid.linelMap.begin();it!=grid.linelMap.end();++it)
    {
        const Linel& l = it->second;

        PixelIncidence p1(l.p1,Internal::positiveIncident(l,l.p1));
        PixelIncidence p2(l.p2,Internal::positiveIncident(l,l.p2));

        ppi[l] = std::vector<PixelIncidence>({p1,p2});
    }
}

void ClosedAndConnected::Internal::edgeIncident(PartialEdgeIncidence& pei,
                                                const Grid &grid)
{
    for(auto it=grid.edgeMap.begin();it!=grid.edgeMap.end();++it)
    {
        const Edge& e = it->second;
        EdgeIncidence e1(e,Internal::positiveIncident(e.l1,e));

        if(pei.find(e.l1)==pei.end()) pei[e.l1] = std::vector<EdgeIncidence>();
        pei[e.l1].push_back(e1);
    }

}

void ClosedAndConnected::closedConnectedContraints(LinelConstraints& constraints,
                                                   const Grid &grid)
{
    typedef std::pair<Linel,LinelIncidence> Element;

    Internal::PartialPixelIncidence ppi;
    Internal::pixelIncident(ppi,grid);

    Internal::PartialEdgeIncidence pei;
    Internal::edgeIncident(pei,grid);

    for(auto it=grid.linelMap.begin();it!=grid.linelMap.end();++it)
    {
        const std::vector<PixelIncidence>& pv = ppi.at(it->second);
        const std::vector<EdgeIncidence>& ev = pei.at(it->second);

        constraints.insert( Element(it->second,
                                    LinelIncidence (pv.at(0),
                                                    pv.at(1),
                                                    ev.at(0),
                                                    ev.at(1)
                                    )));
    }
}