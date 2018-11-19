#ifndef LPMODEL_PAIRWISEITERATOR_H
#define LPMODEL_PAIRWISEITERATOR_H

#include <LPModel/initialization/model/Parameters.h>

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            namespace IteratorManager
            {
                typedef Initialization::Parameters Parameters;
                typedef DGtal::Z2i::Point Point;

                typedef std::function<void(const Point&,const Point&)> BinaryCallback;
                typedef std::function<void(const Point&,const Point&,const Point&)> TernaryCallback;

                void run(const Parameters& prm,
                         const BinaryCallback& bcbk,
                         const TernaryCallback& tcbk);
            }
        }
    }
}

#endif //LPMODEL_PAIRWISEITERATOR_H
