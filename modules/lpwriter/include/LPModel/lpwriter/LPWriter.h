#ifndef LPMODEL_LPWRITER_H
#define LPMODEL_LPWRITER_H

#include <fstream>
#include "LPModel/constraints/ClosedAndConnected.h"

namespace LPModel
{
    namespace LPWriter
    {
        typedef Constraints::ClosedAndConnected::LinelConstraints LinelConstraints;


        typedef Constraints::ClosedAndConnected::Linel Linel;
        typedef Constraints::ClosedAndConnected::LinelIncidence LinelIncidence;

        typedef Constraints::ClosedAndConnected::Pixel Pixel;
        typedef Constraints::ClosedAndConnected::PixelIncidence PixelIncidence;
        typedef Constraints::ClosedAndConnected::EdgeIncidence EdgeIncidence;

        void writePixel(std::ofstream& ofs,const PixelIncidence& pi);
        void writeEdge(std::ofstream &ofs, const EdgeIncidence &ei);

        void writeConstraint(std::ofstream& ofs,
                             int& cIndexStart,
                             const LinelConstraints& lc);
    }
}

#endif //LPMODEL_LPWRITER_H
