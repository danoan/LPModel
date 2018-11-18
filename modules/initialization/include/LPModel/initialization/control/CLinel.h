#ifndef LPMODEL_CLINEL_H
#define LPMODEL_CLINEL_H

#include "DGtal/helpers/StdDefs.h"

#include "../model/Linel.h"
#include "../model/Pixel.h"

namespace LPModel
{
    namespace Initialization
    {
        namespace CLinel
        {
            typedef DGtal::Z2i::Domain Domain;
            typedef DGtal::Z2i::KSpace KSpace;

            typedef std::set<Linel> LinelSet;
            typedef std::map<int,Pixel> PixelMap;

            void createLinelSet(LinelSet& lnlSet,
                                const Domain& domain,
                                const PixelMap& pixelMap);
        }
    }
}
#endif //LPMODEL_CLINEL_H
