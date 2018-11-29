#ifndef LPMODEL_TERMS_SQC_CONSTANTS_H
#define LPMODEL_TERMS_SQC_CONSTANTS_H

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
                typedef std::map<Point,double> ConstantContribution;
                typedef std::map<Point,double> UnaryContribution;
                

                Constants(const double W,
                          const double C,
                          const double F,
                          const double R,
                          const ConstantContribution& constantContribution,
                          const UnaryContribution& unaryContribution):W(W),C(C),F(F),R(R),
                                                                      constantContribution(constantContribution),
                                                                      unaryContribution(unaryContribution)
                {}

                const double W;
                const double C;
                const double F;
                const double R;

                const UnaryContribution unaryContribution;
                const ConstantContribution constantContribution;

            };
        }
    }
}

#endif //LPMODEL_TERMS_SQC_CONSTANTS_H
