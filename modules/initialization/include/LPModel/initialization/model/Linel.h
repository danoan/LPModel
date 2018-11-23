#ifndef LPMODEL_LINEL_H
#define LPMODEL_LINEL_H

#include "Pixel.h"

namespace LPModel
{
    namespace Initialization
    {
        struct Linel
        {
            enum LinelOrientation{Left,Down,Right,Up};

            typedef DGtal::Z2i::Point KPoint;
            typedef std::map<KPoint,Linel> LinelMap;
            typedef std::pair<KPoint,Linel> MapElement;

            Linel(const int x,
                  const int y,
                  const Pixel& p1,
                  const Pixel& p2,
                  const LinelOrientation& orientation,
                  const unsigned long linelIndex): x(x),
                                                   y(y),
                                                   p1(p1),
                                                   p2(p2),
                                                   orientation(orientation),
                                                   linelIndex(linelIndex)
            {}

            bool operator<(const Linel& other) const
            {
                return this->linelIndex < other.linelIndex;
            }

            const int x,y;
            const Pixel &p1,&p2;
            const LinelOrientation orientation;
            const unsigned long linelIndex;

            friend std::ostream& operator<<(std::ostream& os, const Linel& linel);
        };
    }
}

#endif //LPMODEL_LINEL_H
