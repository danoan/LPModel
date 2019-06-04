#ifndef LPMODEL_TERMSLINEL_SQC_LINEL_BINARYHANDLE_H
#define LPMODEL_TERMSLINEL_SQC_LINEL_BINARYHANDLE_H

#include <DGtal/helpers/StdDefs.h>
#include <LPModel/initialization/model/Parameters.h>
#include <LPModel/initialization/model/Grid.h>
#include <LPModel/terms/sqc/model/Constants.h>
#include "LPModel/terms/sqc/adapters/linel/model/LinelContribution.h"

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
                    class BinaryHandle
                    {
                    public:
                        typedef DGtal::Z2i::KSpace KSpace;
                        typedef DGtal::Z2i::Point Point;

                        typedef SCaBOliC::Core::MultiIndex<Point> PointMultiIndex;
                        typedef std::map<PointMultiIndex,double> BinaryMap;

                        typedef SquaredCurvature::Constants Constants;
                        typedef Initialization::Parameters Parameters;


                        BinaryHandle(BinaryMap& bm,
                                     const Constants& sqc):bm(bm),
                                                           sqc(sqc)
                        {}

                        void operator()(const PointMultiIndex& mi);

                    private:
                        const Constants& sqc;
                        BinaryMap& bm;
                    };
                }
            }

        }
        
    }
}

#endif //LPMODEL_TERMS_SQC_LINEL_BINARYHANDLE_H
