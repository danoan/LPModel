#ifndef LPMODEL_CLINEL_H
#define LPMODEL_CLINEL_H

#include "DGtal/helpers/StdDefs.h"

#include "LPModel/initialization/model/Linel.h"
#include "LPModel/initialization/model/Pixel.h"

namespace LPModel
{
    namespace Initialization
    {
        namespace CLinel
        {
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::KSpace KSpace;
            typedef DGtal::Z2i::Point KPoint;

            typedef Linel::LinelMap LinelMap;
            typedef Pixel::PixelMap PixelMap;

            void createLinelSet(LinelMap& linelMap,
                                const Domain& domain,
                                const PixelMap& pixelMap);

            namespace Internal
            {
                struct _Linel
                {
                    _Linel(const KPoint& linelCoord,
                           const KPoint& outRangePixelCoord,
                           const Linel::LinelOrientation orientation): orientation(orientation),
                                                                       pCoord1(outRangePixelCoord),
                                                                       pCoord2(outRangePixelCoord),
                                                                       linelCoord(linelCoord)
                    {}

                    KPoint pCoord1,pCoord2;
                    KPoint linelCoord;
                    Linel::LinelOrientation orientation;
                };

                struct SignedKPoint
                {
                    SignedKPoint(const KPoint& kPoint,
                                 const bool pos):coord(kPoint),
                                                 pos(pos)
                    {}

                    const KPoint coord;
                    const bool pos;
                };

                typedef std::map<DGtal::Z2i::Point,_Linel> AuxLinelMap;
                typedef std::pair<DGtal::Z2i::Point,_Linel> AuxMapElement;
                typedef std::vector<SignedKPoint> IncidentLinels;

                void auxiliaryMap(AuxLinelMap& auxLinelMap,
                                  const Domain& domain,
                                  const PixelMap& pixelMap);

                IncidentLinels incidentLinels(const KPoint& pixel);
                KPoint findOutRangePixelCoord(const PixelMap& pixelMap);


            }
        }
    }
}
#endif //LPMODEL_CLINEL_H
