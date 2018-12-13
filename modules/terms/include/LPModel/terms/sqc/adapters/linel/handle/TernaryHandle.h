#ifndef LPMODEL_SQC_TERMS_LINEL_TERNARYHANDLE_H
#define LPMODEL_SQC_TERMS_LINEL_TERNARYHANDLE_H

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
                        typedef LinelContribution::PointMultiIndex PointMultiIndex;
                        typedef std::map<PointMultiIndex,double> TernaryMap;
                        
                        typedef Initialization::Parameters Parameters;
                        typedef SquaredCurvature::Constants Constants;

                        typedef DGtal::Z2i::KSpace KSpace;
                        typedef DGtal::Z2i::Point Point;

                        TernaryHandle(TernaryMap& tm,
                                      const Constants& sqc):tm(tm),
                                                            sqc(sqc)
                        {}

                        void operator()(const Point& p1, const Point& p2, const Point &p3);

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
