#include <DIPaCUS/derivates/Misc.h>
#include "LPModel/nonlinopt/activeSet/ActiveSetSolver.h"

using namespace LPModel::NonLinOpt;

ActiveSetSolver::ActiveSetSolver(const Grid& grid, const Term& term):grid(grid),term(term)
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

void ActiveSetSolver::buildConstraintsMatrix()
{
    Constraints::ClosedAndConnected::LinelConstraints lc;
    Constraints::ClosedAndConnected::closedConnectedContraints(lc,grid);

    CM.resize(lc.size()+2*numVars,numVars);
    CM.setZero();

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
    equalities = CM.block(0,0,fic,numVars);

    activeFilter.resize(2*numVars);
    activeFilter.setZero();
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

ActiveSetSolver::Matrix ActiveSetSolver::activeSet(Vector& x)
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

double ActiveSetSolver::maxDeplacementOnDk(const Vector& x, const Vector& dk)
{
    //For those that are greater than zero
    FilterMatrix minFilter = (inequalities*dk).array()>Vector::Constant(inequalities.rows(),1,TOLERANCE).array();

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
            if(minFilter(i) && fabs(num(i)) > TOLERANCE)
            {
                pot = num(i)/den(i);
                if(pot < m)
                {
                    m = pot;
                }
            }
        }
    }

    return m;
}

ActiveSetSolver::Vector ActiveSetSolver::feasibleSolution(const SolutionPairVector& spv)
{
    //Notice that spv may come from a linearization of the formulation. In other words
    //the size of spv may be larger than the number of variables.
    NonLinOpt::ActiveSetSolver::Vector v;
    v.resize(numVars);
    for(auto it=spv.begin();it!=spv.end();++it)
    {
        const unsigned long int& index = it->first;

        if(index < numVars)
        {
            if(index<numPixels) v.coeffRef( it->first - edgeOffset ) = it->second;
            else v.coeffRef( it->first ) = it->second;
        }

    }

    return v;
}

void ActiveSetSolver::gradient(Vector& grad, Vector& x)
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

ActiveSetSolver::Size ActiveSetSolver::countTrue(const FilterMatrix& fm)
{
    Size c=0;
    for(int i=0;i<fm.rows();++i)
    {
        if(fm.coeffRef(i)) ++c;
    }

    return c;
}

ActiveSetSolver::Size ActiveSetSolver::countLowerZeroInequalities(const Vector& v, const Matrix& activeSet)
{
    FilterMatrix result = ( (activeSet*v).array()<=Vector::Zero(activeSet.rows()).array() ).matrix();
    return countTrue(result);
}

bool ActiveSetSolver::testDirection(const Matrix& activeSet, const Vector& curDir, const Vector& deplVector, double step, Size currActive,
                                    const Matrix& projM)
{
    Vector nd1 = curDir + step*deplVector;
    nd1 = projM*nd1;
    nd1 /= nd1.norm();
    Size p1 = countLowerZeroInequalities(nd1,activeSet);

    return p1>currActive;
}

ActiveSetSolver::Vector ActiveSetSolver::findFeasibleDirection(const Vector& grad, const Matrix& activeSet)
{

    Matrix W = equalities.fullPivLu().kernel();
    Matrix Q = W*( (W.transpose()*W).inverse() )*W.transpose();

    Matrix temp;
    temp.resize(1,grad.rows());
    temp.row(0)=grad.transpose();

    Matrix N = temp.fullPivLu().kernel();


    Vector dk = -Q*grad;
    dk /= dk.norm();

    Size p0 = countLowerZeroInequalities(dk,activeSet);
    Size p1;
    Vector nd1;

    int i = -1;
    double step=0.01;


    std::srand(time(NULL));
    std::cout << "*** Find feasible direction" << std::endl;
    while(p0<activeSet.rows())
    {
        std::cout << "Gradient times dk: " <<dk.dot(grad) << std::endl;
        std::cout << p0 << " out of " << activeSet.rows() << " inequalities are lower than zero for dk" << std::endl;

        i = (i+1) % N.cols();
        if(i==0)
        {
            step=-step;
            step*=2;
        }
        if( testDirection(activeSet,dk,N.col(i),step,p0,Q) )
        {
            dk = dk + step*N.col(i);
            dk = Q*dk;
            dk /= dk.norm();
            p1 = countLowerZeroInequalities(dk,activeSet);
            p0=p1;
            step=-0.01;
        }

    }

    return dk;

}

double ActiveSetSolver::minimize(Vector& x, int maxSteps)
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
        if(AS.rows()>0)
        {
            Matrix AK = AS.fullPivLu().kernel();
            Matrix ASt = AS.transpose();

            std::cout << "***Iteration" << std::endl;
            std::cout << "### All x greater or equal than zero: "
                      << ( ( (nextX.array()>=Vector::Zero(numVars).array()).all() )?'T':'F' )
                      << std::endl;


            Matrix potSol = ASt.fullPivLu().solve(grad);
            if( (ASt*potSol-grad).norm() < TOLERANCE )
            {
                if( (potSol.array()<=Vector::Zero(potSol.rows()).array()).all() )
                {
                    std::cout << "A stationary solution was found!\n";
                    break;
                }

                std::cout << "### Gradient is a linear combination of active set matrix column vectors,"
                            "but some of its coordinates are positive" << std::endl;

                dk = findFeasibleDirection(grad,AS);
            }
            else
            {
                std::cout << "### dk will be in the nullspace of active matrix" << std::endl;
                dk = findFeasibleDirection(grad,AS);
            }
        }
        else
        {
            if(grad.norm() < TOLERANCE )
            {
                std::cout << "Stationary solution was found!\n";
                break;
            }else
            {
                dk.setRandom();
                dk /= dk.norm();
            }
        }

        std::cout << "Is dk descent direction: " << ( (dk.dot(grad) < 0)?'T':'F' ) << std::endl;

        w = maxDeplacementOnDk(nextX,dk);
        std::cout << "### Max deplacement of: " << w <<  "\n" << std::endl;

        lbda = linearSearch(nextX,grad,dk,w);
        nextX = nextX + lbda*dk;

        for(int i=0;i<nextX.rows();++i)
        {
            if(nextX.coeff(i)<0)
            {
                nextX.coeffRef(i) = 0;
            }else if(nextX.coeff(i)>1)
            {
                nextX.coeffRef(i) = 1;
            }
        }
        --maxSteps;
    }

    solutionVector = nextX;
    return objective<adouble>(nextX).value();
}

double ActiveSetSolver::linearSearch(const Vector& x, const Vector& grad, const Vector& dk, double startLbda)
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

