#ifndef LPMODEL_TERMSLINEL_SQC_CONSTANTS_H
#define LPMODEL_TERMSLINEL_SQC_CONSTANTS_H

#include <DGtal/helpers/StdDefs.h>
#include "SCaBOliC/Energy/base/MultiIndex.h"

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            struct Constants
            {
                typedef DGtal::Z2i::Point Point;
                typedef SCaBOliC::Core::MultiIndex<Point> MyMultiIndex;
                typedef std::map<Point,double> ConstantContribution;
                typedef std::map<MyMultiIndex,double> UnaryContribution;
                typedef std::map<MyMultiIndex,double> PairwiseContribution;


                Constants(const double W,
                          const double C,
                          const double F,
                          const double R,
                          const ConstantContribution& constantContribution,
                          const UnaryContribution& unaryContribution,
                          const PairwiseContribution& pairwiseContribution):W(W),C(C),F(F),R(R),
                                                                            constantContribution(constantContribution),
                                                                            unaryContribution(unaryContribution),
                                                                            pairwiseContribution(pairwiseContribution)
                {}

                const double W;
                const double C;
                const double F;
                const double R;

                const ConstantContribution constantContribution;
                const UnaryContribution unaryContribution;
                const PairwiseContribution pairwiseContribution;

            };
        }
    }
}

#endif //LPMODEL_TERMS_SQC_CONSTANTS_H
