#ifndef LPMODEL_CLOSEDANDCONNECTED_H
#define LPMODEL_CLOSEDANDCONNECTED_H

#include <vector>

#include <LPModel/initialization/model/Pixel.h>
#include <LPModel/initialization/model/Edge.h>

#include <LPModel/initialization/model/Grid.h>
#include <LPModel/initialization/model/Linel.h>
#include <LPModel/constraints/model/LinelIncidence.h>

namespace LPModel
{
    namespace Constraints
    {
        namespace ClosedAndConnected
        {
            typedef Initialization::Linel Linel;
            typedef Initialization::Pixel Pixel;
            typedef Initialization::Edge Edge;
            typedef Initialization::Grid Grid;

            typedef std::map<Linel,LinelIncidence> LinelConstraints;

            void closedConnectedContraints(LinelConstraints& constraints,
                                           const Grid& grid);

            namespace Internal
            {
                typedef std::map<Linel,std::vector<PixelIncidence> > PartialPixelIncidence;
                typedef std::map<Linel,std::vector<EdgeIncidence> > PartialEdgeIncidence;

                void pixelIncident(PartialPixelIncidence& ppi,
                                   const Grid& grid);

                void edgeIncident(PartialEdgeIncidence& pei,
                                  const Grid &grid);

                bool positiveIncident(const Linel& linel, const Pixel& pixel);
                bool positiveIncident(const Linel &linel, const Edge &edge);


            }
        }
    }
}

#endif //LPMODEL_CLOSEDANDCONNECTED_H
