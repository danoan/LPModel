#ifndef LPMODEL_CTERM_H
#define LPMODEL_CTERM_H


#include <LPModel/initialization/model/Parameters.h>
#include <LPModel/initialization/model/Grid.h>

#include <LPModel/terms/sqc/model/Term.h>
#include <LPModel/terms/sqc/model/Constants.h>

#include <LPModel/terms/sqc/adapters/pointel/model/PointelContribution.h>
#include <LPModel/terms/sqc/adapters/pointel/control/CPointelContribution.h>
#include <LPModel/terms/sqc/adapters/pointel/control/IteratorManager.h>

#include <LPModel/terms/sqc/adapters/linel/model/LinelContribution.h>
#include <LPModel/terms/sqc/adapters/linel/control/CLinelContribution.h>


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
                typedef SquaredCurvature::Constants Constants;

                typedef Internal::Pointel::PointelContribution PointelContribution;
                typedef Internal::Linel::LinelContribution LinelContribution;

                Term setTerm(const Parameters &prm,
                             const Grid& grid,
                             const Constants &sqc);


                namespace Internal
                {
                    typedef DGtal::Z2i::Point Point;
                    typedef LinelContribution::PointMultiIndex PointMultiIndex;

                    bool isPixel(const Point &p);


                    void separate(Point &linel,
                                  Point &pixel,
                                  const PointMultiIndex &pmi);

                    void separate(Point &linel,
                                  Point &pixel1,
                                  Point &pixel2,
                                  const PointMultiIndex &pmi);

                    void setBinaryMap(Term::BinaryMap& bm,
                                      const LinelContribution& lctbr,
                                      const Grid& grid);

                    void setTernaryMap(Term::TernaryMap& tm,
                                       const LinelContribution& lctbr,
                                       const Grid& grid);
                }

            }
        }
    }
}


#endif //LPMODEL_CTERM_H
