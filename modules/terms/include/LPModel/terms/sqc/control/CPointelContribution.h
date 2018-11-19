#ifndef LPMODEL_CPOINTELCONTRIBUTION_H
#define LPMODEL_CPOINTELCONTRIBUTION_H

#include "LPModel/initialization/model/Parameters.h"
#include "LPModel/initialization/model/Grid.h"

#include <LPModel/terms/sqc/model/PointelContribution.h>
#include <LPModel/terms/sqc/model/Constants.h>

#include <LPModel/terms/sqc/control/IteratorManager.h>

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            namespace CPointelContribution
            {
                typedef DGtal::Z2i::Point Point;
                typedef DGtal::Z2i::KSpace KSpace;

                typedef PointelContribution::PointMultiIndex PointMultiIndex;
                typedef std::map<PointMultiIndex,double> GeneralMap;

                typedef Initialization::Parameters Parameters;

                class BinaryHandle
                {
                public:
                    typedef DGtal::Z2i::KSpace KSpace;
                    typedef std::map<PointMultiIndex,double> BinaryMap;

                    BinaryHandle(BinaryMap& bm,
                                 const Parameters& prm,
                                 const Constants& sqc):bm(bm),
                                                       prm(prm),
                                                       sqc(sqc){}

                    void operator()(const Point& p1, const Point& p2);

                private:
                    const Parameters& prm;
                    const Constants& sqc;
                    BinaryMap& bm;
                };

                class TernaryHandle
                {
                public:
                    typedef DGtal::Z2i::KSpace KSpace;
                    typedef std::map<PointMultiIndex,double> TernaryMap;

                    TernaryHandle(TernaryMap& tm,
                                  const Parameters& prm,
                                  const Constants& sqc):tm(tm),
                                                        sqc(sqc),
                                                        prm(prm){}

                    void operator()(const Point& p1, const Point& p2, const Point &p3);

                private:
                    const Parameters& prm;
                    const Constants& sqc;
                    TernaryMap& tm;
                };


                PointelContribution setContribution(const Parameters &prm,
                                                    const Constants &sqc);


                double binaryContribution(const KSpace& kspace,
                                          const PointelContribution& pctbr,
                                          const PointMultiIndex& multiIndex);

                double ternaryContribution(const KSpace& kspace,
                                           const PointelContribution& pctbr,
                                           const PointMultiIndex& multiIndex);
            }
        }
    }
}

#endif //LPMODEL_CPOINTELCONTRIBUTION_H
