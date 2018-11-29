#ifndef LPMODEL_SQC_TERMS_POINTEL_TERNARYHANDLE_H
#define LPMODEL_SQC_TERMS_POINTEL_TERNARYHANDLE_H

#include <DGtal/helpers/StdDefs.h>
#include <LPModel/initialization/model/Parameters.h>
#include <LPModel/initialization/model/Grid.h>
#include <LPModel/terms/sqc/adapters/pointel/model/PointelContribution.h>
#include <LPModel/terms/sqc/model/Constants.h>

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            namespace Internal
            {
                namespace Pointel
                {
                    class TernaryHandle
                    {
                    public:
                        typedef PointelContribution::PointMultiIndex PointMultiIndex;
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

#endif //LPMODEL_SQC_TERMS_POINTEL_TERNARYHANDLE_H
