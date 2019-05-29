#include <iostream>

#include "LPModel/glpk/glpk.h"
#include "LPModel/types/types.h"
#include "LPModel/initialization/API.h"
#include "LPModel/terms/sqc/CSqc.h"
#include "LPModel/linearization/model/Linearization.h"
#include "LPModel/lpwriter/LPWriter.h"
#include "LPModel/solassign/model/SolutionAssignment.h"
#include "LPModel/nonlinopt/activeSet/ActiveSetSolver.h"
#include "LPModel/utils/compUtils.h"
#include "LPModel/utils/dispUtils.h"

using namespace LPModel::Initialization;
using namespace LPModel::Terms;
using namespace LPModel::LPWriter;
using namespace LPModel::GLPK;
using namespace LPModel::SolutionAssignment;
using namespace LPModel::NonLinOpt;

template  <typename TSpace>
struct IntersectionFunctor
{
public:
    typedef IntersectionFunctor<TSpace> Self;
    typedef TSpace Space;
    typedef typename Space::RealVector RealVector;
    typedef typename RealVector::Component Component;
    typedef Component Argument;
    typedef Component Quantity;
    typedef Quantity Value;

//    BOOST_CONCEPT_ASSERT(( concepts::CSpace<TSpace> ));
//    BOOST_STATIC_ASSERT(( Space::dimension == 2 ));

    Value operator()( const Argument& arg ) const
    {
        Quantity cp_quantity = arg;
        return cp_quantity;
    }

    void init( Component h, Component r )
    {}

};

template<class TLinelIterator>
void alok(const KSpace& kspace, const DigitalSet& digShape, TLinelIterator begin, TLinelIterator end, double h=1.0)
{
    double re_convolution_kernel = 3.0; // Euclidean radius of the convolution kernel. Set by user.

    double half = DIPaCUS::Shapes::ball(h,0,0,re_convolution_kernel).size()/2.0;

    typedef IntersectionFunctor<DGtal::Z2i::Space> MyIICurvatureFunctor;
    typedef DGtal::IntegralInvariantVolumeEstimator< KSpace, DigitalSet, MyIICurvatureFunctor > MyIICurvatureEstimator;
    typedef MyIICurvatureFunctor::Value Value;

    MyIICurvatureFunctor curvatureFunctor; /// Functor used to convert volume -> curvature
    curvatureFunctor.init( h, re_convolution_kernel ); // Initialisation for a grid step and a given Euclidean radius of convolution kernel

    MyIICurvatureEstimator curvatureEstimator( curvatureFunctor );
    curvatureEstimator.attach( kspace, digShape); /// Setting a KSpace and a predicate on the object to evaluate
    curvatureEstimator.setParams( re_convolution_kernel/h ); /// Setting the digital radius of the convolution kernel
    curvatureEstimator.init( h, begin, end ); /// Initialisation for a given h


    TLinelIterator it = begin;
    while(it!=end)
    {
        std::cout <<  curvatureEstimator.eval(it) << std::endl;
        ++it;
    }
}

SolutionPairVector initialFeasibleSolution(const Parameters& prm, const Grid& grid, const Term& scTerm)
{
    FixedLabels fixedLabels;
    for(auto it = grid.pixelMap.begin();it!=grid.pixelMap.end();++it)
    {
        if(it->second.ct==Pixel::Variable)
            fixedLabels[it->second.varIndex] = 1;
    }

    Index nextIndex = grid.pixelMap.size()-3+grid.edgeMap.size();
    LPModel::Linearization<Term::UIntMultiIndex,double> linearization(nextIndex);
    linearization.linearize(scTerm.binaryMap);
    linearization.coupledLinearization(scTerm.ternaryMap);

    std::string outputLPFormulationPath = "/home-local/dantu1/GIT/PhD/LPModel/lab/test/objective-value/output/formulation.lp";
    writeLP(outputLPFormulationPath,prm,grid,scTerm.unaryMap,linearization,LPModel::RELAXATION_NONE,fixedLabels);

    std::string outputLPSolution = "/home-local/dantu1/GIT/PhD/LPModel/lab/test/objective-value/output/solution.lp";
    glpk_solver(outputLPFormulationPath,outputLPSolution);

    SolutionPairVector spv = solutionPairVector(outputLPSolution,prm,grid);
    return spv;
}

bool countLinel(const Grid& grid, const Linel& linel, const SolutionPairVector& spv)
{
    for(auto it=grid.edgeMap.begin();it!=grid.edgeMap.end();++it)
    {
        if(spv.at(it->first).second==1)
        {
            if( it->second.l1.linelIndex == linel.linelIndex ) return true;
        }
    }

    return false;
}

double sqcEstimation(const Parameters& prm,const Grid& grid, const SolutionPairVector& spv)
{
    typedef DGtal::Z2i::Domain Domain;
    typedef DGtal::Z2i::Point Point;

    DigitalSet tempBall( Domain( 2*Point(-prm.radius,-prm.radius), 2*Point(prm.radius,prm.radius) ) );
    tempBall = DIPaCUS::Shapes::ball(1.0,0,0,prm.radius);

    double W = 0;
    double C = tempBall.size() / 2.0;
    double F = 9.0 / pow(prm.radius, 6.0);
    double R = prm.radius;

    DigitalSet trustPlusOpt = prm.odrModel.trustFRG;
    trustPlusOpt.insert(prm.odrModel.optRegion.begin(),prm.odrModel.optRegion.end());

    DIPaCUS::Misc::DigitalBallIntersection DBI = prm.handle.intersectionComputer(trustPlusOpt);

    Domain domain = prm.odrModel.trustFRG.domain();
    DigitalSet temp(domain);

    double s=0;
    int c=0;
    for(auto it=grid.linelMap.begin();it!=grid.linelMap.end();++it)
    {
        if(countLinel(grid,it->second,spv))
        {
            temp.clear();
            DBI(temp, Point(it->second.x,it->second.y));

            double Ij = temp.size();
            double k = Ij - C;

            s += pow(k,2)*F;
            std::cout << k << std::endl;
            ++c;
        }
    }

    return s;

}

int main()
{
    typedef DIPaCUS::Shapes::DigitalSet DigitalSet;
    int levels = 0;

    DigitalSet dsInput = DIPaCUS::Shapes::square(1.0);
    Parameters prm = API::initParameters(dsInput,
                                         levels);
    Grid grid = API::createGrid(prm.odrModel.optRegion,
                                prm);

    LPModel::Utils::exportODRModel(prm,"/home-local/dantu1/GIT/PhD/LPModel/lab/test/objective-value/output/odrModel.eps");

    Term scTerm = SquaredCurvature::API::prepare(prm,grid,1.0);

    SolutionPairVector spv = initialFeasibleSolution(prm,grid,scTerm);


    ActiveSetSolver solverAS(grid,scTerm);
    ActiveSetSolver::Vector v = solverAS.feasibleSolution(spv);
    adouble objectiveValue = solverAS.objective<adouble>(v)*scTerm.constants.at("weight-factor");

    sqcEstimation(prm,grid,spv);

    DGtal::Z2i::KSpace kspace;
    kspace.init(dsInput.domain().lowerBound(),dsInput.domain().upperBound(),true);
    DGtal::Z2i::Curve curve;
    DIPaCUS::Misc::computeBoundaryCurve(curve,dsInput);
    alok(kspace,dsInput,curve.begin(),curve.end());
    exit(1);

    std::cout << "Energy value: " << LPModel::Utils::sumSQC(dsInput) << std::endl;
    std::cout << "Objective value: " << objectiveValue << std::endl;


    return 0;
}