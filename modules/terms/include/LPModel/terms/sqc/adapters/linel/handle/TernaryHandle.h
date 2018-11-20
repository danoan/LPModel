#ifndef LPMODEL_TERNARYHANDLE_H
#define LPMODEL_TERNARYHANDLE_H

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
                    class TernaryHandle
                    {
                    public:
                        typedef DGtal::Z2i::Point Point;

                        typedef LinelContribution::TernaryMap TernaryMap;
                        typedef Internal::Pointel::PointelContribution PointelContribution;

                    public:
                        TernaryHandle(TernaryMap& tm,
                                     const PointelContribution& pctbr):tm(tm),
                                                                       pctbr(pctbr)
                        {}

                        void operator()(const Point& linel,
                                        const Point& pointel,
                                        const Point& pixel1,
                                        const Point& pixel2);

                    private:
                        const PointelContribution& pctbr;
                        TernaryMap& tm;
                    };
                }
            }
        }
    }
}

#endif //LPMODEL_TERNARYHANDLE_H
