#ifndef LPMODEL_TERMS_SQC_LINEL_BINARYHANDLE_H
#define LPMODEL_TERMS_SQC_LINEL_BINARYHANDLE_H

#include <DGtal/helpers/StdDefs.h>
#include <LPModel/terms/sqc/adapters/pointel/model/PointelContribution.h>
#include <LPModel/terms/sqc/adapters/linel/model/LinelContribution.h>

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
                        typedef DGtal::Z2i::Point Point;

                        typedef LinelContribution::BinaryMap BinaryMap;
                        typedef Internal::Pointel::PointelContribution PointelContribution;

                    public:
                        BinaryHandle(BinaryMap& bm,
                                     const PointelContribution& pctbr):bm(bm),
                                                                       pctbr(pctbr)
                        {}

                        void operator()(const Point& linel,
                                        const Point& pointel,
                                        const Point& pixel);

                    private:
                        const PointelContribution& pctbr;
                        BinaryMap& bm;
                    };
                }
            }
        }
    }
}

#endif //LPMODEL_TERMS_SQC_LINEL_BINARYHANDLE_H
