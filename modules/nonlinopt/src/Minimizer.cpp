#include <DIPaCUS/derivates/Misc.h>
#include "LPModel/nonlinopt/Minimizer.h"

using namespace LPModel::NonLinOpt;

Minimizer::Minimizer(const Grid& grid, const Term& term):grid(grid),term(term)
{
    numPixels=0;
    numEdges=0;
    for(auto it=grid.pixelMap.begin();it!=grid.pixelMap.end();++it)
    {
        if( it->second.ct==Initialization::Pixel::Variable ) ++numPixels;
    }

    edgeOffset=grid.edgeMap.begin()->second.varIndex;
    for(auto it=grid.edgeMap.begin();it!=grid.edgeMap.end();++it)
    {
        if(it->second.varIndex < edgeOffset) edgeOffset = it->second.varIndex;
        ++numEdges;
    }

    edgeOffset -= numPixels;

    buildKernel();
}

void Minimizer::buildKernel()
{
    Constraints::ClosedAndConnected::LinelConstraints lc;
    Constraints::ClosedAndConnected::closedConnectedContraints(lc,grid);

    CM.resize(lc.size(),numPixels+numEdges);

    Size currRow = 0;
    for(auto it=lc.begin();it!=lc.end();++it,++currRow)
    {
        const Linel& linel = it->first;
        const LinelIncidence linc = it->second;

        if(linc.pixel1.pixel.ct==Pixel::CellType::Variable)
            CM.coeffRef(currRow,linc.pixel1.pixel.varIndex) = linc.pixel1.posIncidence?1:-1;

        if(linc.pixel2.pixel.ct==Pixel::CellType::Variable)
            CM.coeffRef(currRow,linc.pixel2.pixel.varIndex) = linc.pixel2.posIncidence?1:-1;

        CM.coeffRef(currRow,linc.edge1.edge.varIndex-edgeOffset) = linc.edge1.posIncidence?1:-1;
        CM.coeffRef(currRow,linc.edge2.edge.varIndex-edgeOffset) = linc.edge2.posIncidence?1:-1;
    }

    kernel = CM.fullPivLu().kernel();
}

Minimizer::Vector Minimizer::feasibleSolution(const SolutionPairVector& spv)
{
    NonLinOpt::Minimizer::Vector v;
    v.resize(spv.size());
    for(auto it=spv.begin();it!=spv.end();++it)
    {
        const unsigned long int& index = it->first;

        if(index>numPixels) v.coeffRef( it->first - edgeOffset ) = it->second;
        else v.coeffRef( it->first ) = it->second;

    }

    return v;
}

void Minimizer::gradient(Vector& grad, Vector& x)
{
    std::vector<adouble> ax(x.size());
    adept::set_values(&ax[0],x.size(),&x[0]);

    stack.new_recording();

    adouble y = objective<adouble>(ax);
    std::cout << "Current y: " << y.value() << std::endl;

    y.set_gradient(1.0);
    stack.compute_adjoint();
    for(Size c=0;c<numPixels+numEdges;++c)
    {
        grad.coeffRef(c) = ax[c].get_gradient();
    }
}

double Minimizer::minimize(Vector& x, int maxSteps)
{
    double lbda;

    Vector dk,grad;
    dk.resize(numPixels+numEdges);
    grad.resize(numPixels+numEdges);

    Vector nextX = x;
    while(maxSteps>0)
    {
        std::cout << CM*nextX << std::endl;

        gradient(grad,nextX);
        if( (kernel.transpose()*grad).norm() < TOLERANCE  ) break;

        dk = -kernel*kernel.transpose()*grad;
        lbda = linearSearch(nextX,grad,dk);

        nextX = nextX + lbda*dk;

        --maxSteps;
    }

    return objective<adouble>(nextX).value();
}

double Minimizer::linearSearch(const Vector& x, const Vector& grad, const Vector& dk)
{
    int cmax=10;
    double lbda=1.0;
    double fxk = objective<double>(x);
    while( objective<double>( x + lbda*dk ) > fxk + alpha*lbda*grad.dot(dk) && cmax>0 )
    {
        lbda/=2;
        --cmax;
    }

    return lbda;
}

