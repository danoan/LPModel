#ifndef LPMODEL_COMPUTILS_H
#define LPMODEL_COMPUTILS_H

#include <geoc/api/gridCurve/Curvature.hpp>

namespace LPModel
{
    namespace Utils
    {
        typedef DGtal::Z2i::DigitalSet DigitalSet;

        double sumSQC(const DigitalSet& ds);
    }
}

#endif //LPMODEL_COMPUTILS_H
