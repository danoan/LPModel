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
        class MinimizerAR
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

            struct Deplacement
            {
                Deplacement(const Matrix::ColXpr v):sigma(1),
                                                    low(-std::numeric_limits<double>::max()),
                                                    high(std::numeric_limits<double>::max()),
                                                    state(UndefRange),
                                                    action(Sum),
                                                    v(v),
                                                    ic(0),
                                                    inc(0){}

                double defRange()
                {
                    while(true)
                    {
                        if(action==Sum)
                        {
                            sigma = sigma_s + ic*inc;
                            if(sigma>=high) action=Subtract;
                            else return sigma;
                        }else
                        {
                            sigma = sigma_s -ic*inc;
                            if( sigma<=low )
                            {
                                inc/=2.0;
                                ic=0;
                                action=Sum;
                            }else
                            {
                                return sigma;
                            }
                        }
                    }
                }

                double undefRange()
                {
                    while(true)
                    {
                        if(action==Sum)
                        {
                            action=Subtract;
                            if(sigma<high)
                            {
                                sigma*=2;
                                return sigma;
                            }
                        }else
                        {
                            action=Sum;
                            if(-sigma>low)
                            {
                                sigma*=2;
                                return -sigma;
                            }
                        }
                    }
                }

                double next()
                {
                    if(state==DefRange) return defRange();
                    else return undefRange();
                }

                void setRange()
                {
                    state = DefRange;
                    range = high-low;
                    sigma_s = range/2.0;
                    action = Sum;
                    inc = range/2.0;
                }

                void setHigh()
                {
                    if(state==DefRange) return;
                    high = sigma;
                    if(low!=-std::numeric_limits<double>::max()) setRange();
                }

                void setLow()
                {
                    if(state==DefRange) return;
                    low = sigma;
                    if(high!=std::numeric_limits<double>::max()) setRange();
                }

                double sigma,sigma_s,range;
                double low,high;

                enum{DefRange,UndefRange} state;
                enum{Sum,Subtract} action;

                Size ic;
                double inc;

                const Matrix::ColXpr v;
            };


        public:
            MinimizerAR(const Grid& grid, const Term& term);
            Vector feasibleSolution(const SolutionPairVector& spv);

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
            Matrix inequalities;
            Vector activeFilter;

            Matrix eqKernel;


            Size numPixels,numEdges,numVars;
            Size edgeOffset;
            Size fic;

            const Grid& grid;
            const Term& term;

            const double TOLERANCE = 1.0e-6;
            const double alpha = 0.1;


        };
    }

}


#endif //LPMODEL_MINIMIZERAR_H
