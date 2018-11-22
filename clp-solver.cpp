#include "coin/OsiClpSolverInterface.hpp"

int main(int argc, char* argv[])
{
    std::string lpFile = "../output/square/h2/lp-program.txt";

    OsiSolverInterface* osi = new OsiClpSolverInterface();
    osi->readLp(lpFile.c_str());


    osi->initialSolve();

    if( osi->isProvenOptimal() )
    {
        int n = osi->getNumCols();
        const double* solution = osi->getColSolution();
        std::cout << *solution << std::endl;
    }
    else
    {
        std::cout << "Optimal solution not found!" << std::endl;
    }


    return 0;
}