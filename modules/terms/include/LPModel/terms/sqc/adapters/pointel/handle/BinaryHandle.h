#ifndef LPMODEL_BINARYHANDLE_H
#define LPMODEL_BINARYHANDLE_H

#include <DGtal/helpers/StdDefs.h>
#include <LPModel/initialization/model/Parameters.h>
#include <LPModel/initialization/model/Grid.h>
#include <LPModel/terms/sqc/model/Constants.h>
#include "LPModel/terms/sqc/adapters/pointel/model/PointelContribution.h"

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
                    class BinaryHandle
                    {
                    public:
                        typedef PointelContribution::PointMultiIndex PointMultiIndex;
                        typedef std::map<PointMultiIndex,double> BinaryMap;

                        typedef SquaredCurvature::Constants Constants;
                        typedef Initialization::Parameters Parameters;

                        typedef DGtal::Z2i::KSpace KSpace;
                        typedef DGtal::Z2i::Point Point;

                        BinaryHandle(BinaryMap& bm,
                                     const Constants& sqc):bm(bm),
                                                           sqc(sqc)
                        {}

                        void operator()(const Point& p1, const Point& p2);

                    private:
                        const Constants& sqc;
                        BinaryMap& bm;
                    };
                }
            }

        }
        
    }
}

#endif //LPMODEL_BINARYHANDLE_H
