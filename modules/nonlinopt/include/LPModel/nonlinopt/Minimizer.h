#ifndef LPMODEL_NONLINOPT_H
#define LPMODEL_NONLINOPT_H

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
        class Minimizer
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


        public:
            Minimizer(const Grid& grid, const Term& term);

            Vector feasibleSolution(const SolutionPairVector& spv);

            template<class TIterator>
            Vector initialFeasibleSolution(TIterator begin, TIterator end)
            {
                Vector v;
                v.resize(numPixels+numEdges);
                int i=0;
                while(begin!=end)
                {
                    v.coeffRef(i)=*begin;
                    ++begin;
                    ++i;
                }
                assert(i==numPixels+numEdges);
                return v;
            }

            template<class TNUM, class TNUMARRAY>
            TNUM objective( const TNUMARRAY x)
            {
                TNUM res = 0;
                Size index1,index2,index3;
                for(auto it=term.unaryMap.begin();it!=term.unaryMap.end();++it)
                {
                    index1 = *( it->first.begin() ) - edgeOffset;
                    res += x[index1]*it->second;
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

                return res;

            }

            double minimize( Vector& x, int maxSteps);
            double linearSearch(const Vector& x, const Vector& grad, const Vector& dk);



        private:
            void buildKernel();
            void gradient(Vector& grad, Vector& x);

        private:
            adept::Stack stack;

            Matrix kernel;
            Matrix CM;

            Size numPixels,numEdges;
            Size edgeOffset;

            const Grid& grid;
            const Term& term;

            const double TOLERANCE = 1.0e-6;
            const double alpha = 0.1;


        };
    }

}


#endif //LPMODEL_NONLINOPT_H
