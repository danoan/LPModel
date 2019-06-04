#ifndef LPMODEL_TERMSLINEL_SQC_LINEL_ITERATORMANAGER_H
#define LPMODEL_TERMSLINEL_SQC_LINEL_ITERATORMANAGER_H

#include <DGtal/helpers/StdDefs.h>
#include <LPModel/initialization/model/Parameters.h>
#include <SCaBOliC/Energy/ISQ/Terms/SquaredCurvature/LinelCoefficientsComputer.h>
#include <LPModel/terms/model/MultiIndex.h>

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            namespace Internal
            {
                namespace Linel
                {
                    namespace IteratorManager
                    {
                        typedef Initialization::Parameters Parameters;
                        typedef DGtal::Z2i::Point Point;
                        typedef SCaBOliC::Core::MultiIndex<Point> PointMultiIndex;

                        typedef std::function<void(const PointMultiIndex& pmi)> BinaryCallback;
                        typedef std::function<void(const PointMultiIndex& pmi)> TernaryCallback;

                        void run(const Parameters& prm,
                                 const BinaryCallback& bcbk,
                                 const TernaryCallback& tcbk);
                    }
                }
            }
        }
    }
}

#endif //LPMODEL_TERMS_SQC_LINEL_ITERATORMANAGER_H
