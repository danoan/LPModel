#include <DIPaCUS/derivates/Misc.h>
#include "LPModel/nonlinopt/MinimizerAR.h"

using namespace LPModel::NonLinOpt;

MinimizerAR::MinimizerAR(const Grid& grid, const Term& term):grid(grid),term(term)
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

    numVars = numPixels+numEdges;
    edgeOffset -= numPixels;

    buildConstraintsMatrix();

}

void MinimizerAR::buildConstraintsMatrix()
{
    Constraints::ClosedAndConnected::LinelConstraints lc;
    Constraints::ClosedAndConnected::closedConnectedContraints(lc,grid);

    CM.resize(lc.size()+2*numVars,numVars);

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

    fic = currRow;
    eqKernel = CM.block(0,0,fic,numVars).fullPivLu().kernel();

    activeFilter.resize(2*numVars);
    //Greater than Zero and smaller than One
    for(auto it=grid.pixelMap.begin();it!=grid.pixelMap.end();++it)
    {
        if(it->second.ct==Pixel::CellType::Variable)
        {
            //Greater than Zero
            CM.coeffRef(currRow,it->second.varIndex) = -1;
            activeFilter.coeffRef(currRow-fic) = 0;
            //Smaller than One
            CM.coeffRef(numVars+currRow,it->second.varIndex) = 1;
            activeFilter.coeffRef(numVars+(currRow-fic)) = 1;
            ++currRow;
        }
    }

    for(auto it=grid.edgeMap.begin();it!=grid.edgeMap.end();++it,++currRow)
    {
        //Greater than Zero
        CM.coeffRef(currRow,it->second.varIndex-edgeOffset) = -1;
        activeFilter.coeffRef(currRow-fic) = 0;
        //Smaller than One
        CM.coeffRef(numVars+currRow,it->second.varIndex-edgeOffset) = 1;
        activeFilter.coeffRef(numVars + (currRow-fic) ) = 1;
    }

    lbFilter = activeFilter + Vector::Constant(activeFilter.rows(),activeFilter.cols(),-1e-20);
    ubFilter = activeFilter + Vector::Constant(activeFilter.rows(),activeFilter.cols(),1e-20);


    inequalities = CM.block(fic,0,2*numVars,numVars);

}

MinimizerAR::Matrix MinimizerAR::activeSet(Vector& x)
{
    FilterMatrix vLb = ( (inequalities*x).array()>=lbFilter.array() ).matrix();
    FilterMatrix vUb = ( (inequalities*x).array()<=ubFilter.array() ).matrix();
    FilterMatrix v = vLb && vUb;

    Size activeConstraints =0;
    for(int i=0;i<v.size();++i)
    {
        if(v[i])++activeConstraints;
    }

    Matrix subM;
    subM.resize(activeConstraints,numVars);
    Size aIndex=0;
    for(int i=0;i<v.size();++i)
    {
        if(v[i])
        {
            subM.row(aIndex) = inequalities.row(i);
            ++aIndex;
        }
    }

    return subM;
}

double MinimizerAR::maxDeplacementOnDk(const Vector& x, const Vector& dk)
{
    FilterMatrix minFilter = (inequalities*dk).array()>Vector::Zero(inequalities.rows()).array();
    const Matrix& num = activeFilter-inequalities*x;
    const Matrix& den = inequalities*dk;

    double m=1;
    if(minFilter.any())
    {
        int first = 0;
        while (!minFilter(first))++first;
        m = num(first)/den(first);

        double pot=0;
        for(int i=first;i<minFilter.rows();++i)
        {
            if(minFilter(i))
            {
                pot = num(i)/den(i);
                if(pot < m) m = pot;
            }
        }
    }

    return m;
}

MinimizerAR::Vector MinimizerAR::feasibleSolution(const SolutionPairVector& spv)
{
    NonLinOpt::MinimizerAR::Vector v;
    v.resize(spv.size());
    for(auto it=spv.begin();it!=spv.end();++it)
    {
        const unsigned long int& index = it->first;

        if(index>numPixels) v.coeffRef( it->first - edgeOffset ) = it->second;
        else v.coeffRef( it->first ) = it->second;

    }

    return v;
}

void MinimizerAR::gradient(Vector& grad, Vector& x)
{
    std::vector<adouble> ax(x.size());
    adept::set_values(&ax[0],x.size(),&x[0]);

    stack.new_recording();

    adouble y = objective<adouble>(ax);
    std::cout << "Current y: " << y.value() << std::endl;

    y.set_gradient(1.0);
    stack.compute_adjoint();
    for(Size c=0;c<numVars;++c)
    {
        grad.coeffRef(c) = ax[c].get_gradient();
    }
}

MinimizerAR::Size MinimizerAR::countTrue(const FilterMatrix& fm)
{
    Size c=0;
    for(int i=0;i<fm.rows();++i)
    {
        if(fm.coeffRef(i)) ++c;
    }

    return c;
}

MinimizerAR::Size MinimizerAR::countLowerZeroInequalities(const Vector& v, const Matrix& activeSet)
{
    FilterMatrix result = ( (activeSet*v).array()<Vector::Zero(activeSet.rows()).array() ).matrix();
    return countTrue(result);
}

MinimizerAR::Vector MinimizerAR::findFeasibleDirection(const Vector& grad, const Matrix& activeSet)
{
    Matrix temp;
    temp.resize(1,grad.rows());
    temp.row(0)=grad.transpose();

    Matrix W = temp.fullPivLu().kernel();
    std::vector<Deplacement> deplacements;
    for(int i=0;i<W.cols();++i)
    {
        deplacements.push_back(Deplacement(W.col(i)));
    }

    Vector dk = -eqKernel*eqKernel.transpose()*grad;
    dk /= dk.norm();



    Size p0 = countLowerZeroInequalities(dk,activeSet);
    Size p1;
    Vector nd1;
    Deplacement* depl;
    int i = -1;
    int c;


    std::srand(time(NULL));
    std::cout << "*** Find feasible direction" << std::endl;
    while(p0<activeSet.rows())
    {
        std::cout << "Gradient times dk: " <<dk.dot(grad) << std::endl;
        std::cout << p0 << " out of " << activeSet.rows() << " inequalities are lower than zero for dk" << std::endl;

        i = (i+1) % deplacements.size();
        c=0;
        do
        {
            depl = &deplacements[i];
            nd1 = dk + depl->next() * depl->v;
            nd1 /= nd1.norm();
            p1 = countLowerZeroInequalities(nd1,activeSet);

            if (p1 < p0) depl->setHigh();
            else if (p1 > p0) depl->setLow();

            ++c;

        } while (p1 < p0 && c < 5);

        if(c!=5)
        {
            dk = nd1;
            p0 = p1;
        }

    }



    return dk;

}

double MinimizerAR::minimize(Vector& x, int maxSteps)
{
    double lbda,w;

    Vector dk,grad;
    dk.resize(numVars);
    grad.resize(numVars);

    Vector nextX = x;
    while(maxSteps>0)
    {
        gradient(grad,nextX);
        Matrix AS = activeSet(nextX);
        Matrix AK = AS.fullPivLu().kernel();
        Matrix ASt = AS.transpose();

        if(grad.norm() < TOLERANCE) break;
        if( (AK.transpose()*grad).norm() < TOLERANCE  ) break;

        std::cout << "***Iteration" << std::endl;
        std::cout << "### All x greater than zero: "
                  << ( ( (nextX.array()>=Vector::Zero(numVars).array()).all() )?'T':'F' )
                  << std::endl;


        Matrix potSol = ASt.fullPivLu().solve(grad);
        if( (ASt*potSol-grad).norm() < TOLERANCE )
        {
            if( (potSol.array()<Vector::Zero(potSol.rows()).array()).all() ) break;

            std::cout << "### Gradient is in the active matrix image" << std::endl;

            dk = -AK*AK.transpose()*grad;
        }
        else
        {
            std::cout << "### dk will be in the nullspace of active matrix" << std::endl;
            dk = findFeasibleDirection(grad,AS);
            //dk = -AK*AK.transpose()*grad;
        }

        std::cout << "Is dk descent direction: " << ( (dk.dot(grad) < 0)?'T':'F' ) << std::endl;

        w = maxDeplacementOnDk(nextX,dk);
        std::cout << "### Max deplacement of: " << w <<  "\n" << std::endl;

        lbda = linearSearch(nextX,grad,dk,w);
        nextX = nextX + lbda*dk;


        --maxSteps;
    }

    return objective<adouble>(nextX).value();
}

double MinimizerAR::linearSearch(const Vector& x, const Vector& grad, const Vector& dk, double startLbda)
{
    int cmax=10;
    double lbda=startLbda;
    double fxk = objective<double>(x);
    while( objective<double>( x + lbda*dk ) > fxk + alpha*lbda*grad.dot(dk) && cmax>0 )
    {
        lbda/=2;
        --cmax;
    }

    return lbda;
}

