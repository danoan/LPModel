#ifndef LPMODEL_SQC_TERMSLINEL_LINEL_TERNARYHANDLE_H
#define LPMODEL_SQC_TERMSLINEL_LINEL_TERNARYHANDLE_H

#include <DGtal/helpers/StdDefs.h>
#include <LPModel/initialization/model/Parameters.h>
#include <LPModel/initialization/model/Grid.h>
#include <LPModel/terms/sqc/adapters/linel/model/LinelContribution.h>
#include <LPModel/terms/sqc/model/Constants.h>

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
                    class TernaryHandle
                    {
                    public:
                        typedef DGtal::Z2i::KSpace KSpace;
                        typedef DGtal::Z2i::Point Point;

                        typedef SCaBOliC::Core::MultiIndex<Point> PointMultiIndex;
                        typedef std::map<PointMultiIndex,double> TernaryMap;
                        
                        typedef Initialization::Parameters Parameters;
                        typedef SquaredCurvature::Constants Constants;


                        TernaryHandle(TernaryMap& tm,
                                      const Constants& sqc):tm(tm),
                                                            sqc(sqc)
                        {}

                        void operator()(const PointMultiIndex& mi);

                    private:
                        const Constants& sqc;
                        TernaryMap& tm;
                    };
                }
            }

        }
    }
}

#endif //LPMODEL_SQC_TERMS_LINEL_TERNARYHANDLE_H
