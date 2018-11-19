#ifndef LPMODEL_CONSTANTS_H
#define LPMODEL_CONSTANTS_H

#include <DGtal/helpers/StdDefs.h>

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            struct Constants
            {
                typedef DGtal::Z2i::Point Point;
                typedef std::map<Point,double> UnaryContribution;

                Constants(const double W,
                          const double C,
                          const double F,
                          const double R,
                          const UnaryContribution& unaryContribution):W(W),C(C),F(F),R(R),
                                                                      unaryContribution(unaryContribution)
                {}

                const double W;
                const double C;
                const double F;
                const double R;

                const UnaryContribution unaryContribution;

            };
        }
    }
}

#endif //LPMODEL_CONSTANTS_H
