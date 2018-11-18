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

            Linel(const Pixel& p1,
                  const Pixel& p2,
                  const LinelOrientation& orientation,
                  const int linelIndex):p1(p1),
                                        p2(p2),
                                        orientation(orientation),
                                        linelIndex(linelIndex)
            {}

            bool operator<(const Linel& other) const
            {
                return this->linelIndex < other.linelIndex;
            }

            const Pixel &p1,&p2;
            const LinelOrientation orientation;
            const int linelIndex;
        };
    }
}

#endif //LPMODEL_LINEL_H
