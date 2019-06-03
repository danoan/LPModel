#ifndef LPMODEL_MINIMIZERAR_H
#define LPMODEL_MINIMIZERAR_H

#include <limits>
#include <random>

#include "eigen3/Eigen/Dense"
#include "adept.h"

#include <LPModel/initialization/model/Grid.h>
#include <LPModel/constraints/ClosedAndConnected.h>
#include "LPModel/terms/model/Term.h"

using adept::adouble;

namespace LPModel
{
    namespace NonLinOpt
    {
        class ActiveSetSolver
        {
        public:
            typedef unsigned long int Size;

            typedef Eigen::MatrixXd Matrix;
            typedef Eigen::VectorXd Vector;

            typedef Initialization::Grid Grid;
            typedef Terms::Term Term;

            typedef DGtal::Z2i::DigitalSet DigitalSet;

            typedef std::pair<Size,double> SolutionPair;
            typedef std::vector<SolutionPair> SolutionPairVector;

        private:
            typedef Initialization::Pixel Pixel;

            typedef Constraints::ClosedAndConnected::Linel Linel;
            typedef Constraints::ClosedAndConnected::LinelIncidence LinelIncidence;

            typedef DGtal::Z2i::Curve Curve;
            typedef DGtal::Z2i::KSpace KSpace;

            typedef Eigen::Matrix<bool,Eigen::Dynamic,1> FilterMatrix;

        public:
            ActiveSetSolver(const Grid& grid, const Term& term);
            Vector feasibleSolution(const SolutionPairVector& spv);

            bool testDirection(const Matrix& activeSet, const Vector& curDir, const Vector& deplVector, double step, Size currActive);

            template<class TNUM, class TNUMARRAY>
            TNUM objective( const TNUMARRAY x) const
            {
                int c=0;
                TNUM res = 0;
                Size index1,index2,index3;
                for(auto it=term.unaryMap.begin();it!=term.unaryMap.end();++it)
                {
                    index1 = *( it->first.begin() ) - edgeOffset;
                    res += x[index1]*it->second;
                    if( x[index1]*it->second!=0 ) ++c;
                }

                for(auto it=term.binaryMap.begin();it!=term.binaryMap.end();++it)
                {
                    auto itM = it->first.begin();
                    index1 = *itM; ++itM;
                    index2 = *itM - edgeOffset;

                    res += x[index1]*x[index2]*it->second;
                }

                for(auto it=term.ternaryMap.begin();it!=term.ternaryMap.end();++it)
                {
                    auto itM = it->first.begin();
                    index1 = *itM; ++itM;
                    index2 = *itM; ++itM;
                    index3 = *itM - edgeOffset;

                    res += x[index1]*x[index2]*x[index3]*it->second;
                }

                //Force Zero or One solutions
//                for(int i=0;i<numVars;++i)
//                {
//                    res += pow(x[i],2)*pow( (x[i]-1),2)*5;
//                }

                return res;

            }

            double minimize( Vector& x, int maxSteps);
            double linearSearch(const Vector& x, const Vector& grad, const Vector& dk, const double startLbda=1.0);



        private:
            void buildConstraintsMatrix();
            Matrix activeSet(Vector& x);
            double maxDeplacementOnDk(const Vector& x, const Vector& dk);
            Size countLowerZeroInequalities(const Vector& v, const Matrix& activeSet);
            Size countTrue(const FilterMatrix& fm);
            Vector findFeasibleDirection(const Vector& grad, const Matrix& activeSet);


            void gradient(Vector& grad, Vector& x);

        private:
            adept::Stack stack;

            Matrix CM;
            Matrix equalities;
            Matrix inequalities;
            Vector activeFilter,lbFilter,ubFilter;

            Matrix eqKernel;


            Size numPixels,numEdges,numVars;
            Size edgeOffset;
            Size fic;

            const Grid& grid;
            const Term& term;

            const double TOLERANCE = 1.0e-6;
            const double alpha = 0.1;

        public:
            Vector solutionVector;


        };
    }

}


#endif //LPMODEL_MINIMIZERAR_H
