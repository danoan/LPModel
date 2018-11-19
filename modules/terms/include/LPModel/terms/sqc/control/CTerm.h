#ifndef LPMODEL_CTERM_H
#define LPMODEL_CTERM_H


#include <LPModel/initialization/model/Parameters.h>
#include <LPModel/initialization/model/Grid.h>

#include <LPModel/terms/sqc/model/Term.h>
#include <LPModel/terms/sqc/model/Constants.h>
#include <LPModel/terms/sqc/model/PointelContribution.h>

#include <LPModel/terms/sqc/control/CPointelContribution.h>
#include <LPModel/terms/sqc/control/IteratorManager.h>

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            namespace CTerm
            {
                typedef Initialization::Parameters Parameters;
                typedef Initialization::Grid Grid;
                typedef DGtal::Z2i::Point Point;

                class BinaryHandle
                {
                public:
                    typedef DGtal::Z2i::KSpace KSpace;
                    typedef Term::BinaryMap BinaryMap;

                    BinaryHandle(BinaryMap& bm,
                                 const Parameters& prm,
                                 const Grid& grid,
                                 const PointelContribution& pctbr):bm(bm),
                                                                   prm(prm),
                                                                   grid(grid),
                                                                   pctbr(pctbr)
                    {
                        kspace.init(prm.odrModel.domain.lowerBound(),prm.odrModel.domain.upperBound(),true);
                    }

                    void operator()(const Point& p1, const Point& p2);

                private:
                    const Parameters& prm;
                    const Grid& grid;
                    const PointelContribution& pctbr;
                    KSpace kspace;

                    BinaryMap& bm;
                };

                class TernaryHandle
                {
                public:
                    typedef DGtal::Z2i::KSpace KSpace;
                    typedef Term::BinaryMap TernaryMap;

                    TernaryHandle(TernaryMap& tm,
                                  const Parameters& prm,
                                  const Grid& grid,
                                  const PointelContribution& pctbr):tm(tm),
                                                                    prm(prm),
                                                                    grid(grid),
                                                                    pctbr(pctbr)
                    {
                        kspace.init(prm.odrModel.domain.lowerBound(),prm.odrModel.domain.upperBound(),true);
                    }

                    void operator()(const Point& p1, const Point& p2, const Point &p3);

                private:
                    const Parameters& prm;
                    const Grid& grid;
                    const PointelContribution& pctbr;
                    KSpace kspace;

                    TernaryMap& tm;
                };


                Term setTerm(const Parameters &prm,
                             const Grid& grid,
                             const Constants &sqc);

            }
        }
    }
}


#endif //LPMODEL_CTERM_H
