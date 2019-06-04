#ifndef LPMODEL_TERMSLINEL_SQC_CTERM_H
#define LPMODEL_TERMSLINEL_SQC_CTERM_H


#include <LPModel/initialization/model/Parameters.h>
#include <LPModel/initialization/model/Grid.h>

#include <LPModel/initialization/control/CLinel.h>

#include <LPModel/terms/model/Term.h>
#include <LPModel/terms/sqc/model/Constants.h>

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

                typedef Internal::Linel::LinelContribution LinelContribution;

                typedef std::map<DGtal::Z2i::Point,double> LinelFullContrib;

                Term setTerm(const Parameters &prm,
                             const Grid& grid,
                             const Constants &sqc,
                             double weight);


                namespace Internal
                {
                    typedef DGtal::Z2i::Point Point;
                    typedef LinelContribution::PointMultiIndex PointMultiIndex;

                    bool isPixel(const Point &p);
                    bool isLinel(const DGtal::Z2i::Point& p);


                    void separate(Point &linel,
                                  Point &pixel,
                                  const PointMultiIndex &pmi);

                    void separate(Point &linel,
                                  Point &pixel1,
                                  Point &pixel2,
                                  const PointMultiIndex &pmi);

                    void setUnaryMap(Term::UnaryMap& um,
                                     const Constants& sqc,
                                     const Parameters& prm,
                                     const Grid& grid,
                                     double weight);                    

                    void setBinaryMap(Term::BinaryMap& bm,
                                      LinelFullContrib& lfc,
                                      const Constants& sqc,
                                      const LinelContribution& lctbr,
                                      const Grid& grid,
                                      double weight);

                    void setTernaryMap(Term::TernaryMap& tm,
                                       LinelFullContrib& lfc,
                                       const Constants& sqc,
                                       const LinelContribution& lctbr,
                                       const Grid& grid,
                                       double weight);

                    void addBinaryElement(Term::BinaryMap& bm,
                                          const unsigned long n1,
                                          const unsigned long n2,
                                          const double value);

                    void addTernaryElement(Term::TernaryMap& tm,
                                           const unsigned long n1,
                                           const unsigned long n2,
                                           const unsigned long n3,
                                           const double value);

                }

            }
        }
    }
}


#endif //LPMODEL_TERMS_SQC_CTERM_H
