#include "LPModel/glpk/glpk.h"

using namespace LPModel;


void GLPK::glpk_solver(const std::string& lpInputFile, const std::string& outputSolutionFile)
{
    glp_prob* P = glp_create_prob();
    int code = glp_read_lp(P, NULL, lpInputFile.c_str());

    if(code!=0)
    {
        std::cerr << "Error readind lp program" << std::endl;
        exit(code);
    }

    code = glp_simplex(P,NULL);
    if(code!=0)
    {
        std::cerr << "Error dictionaire\n"
                  << "GLP_EBADB: " << GLP_EBADB << "\n"
                  << "GLP_ESING: " << GLP_ESING << "\n"
                  << "GLP_ECOND: " << GLP_ECOND << "\n"
                  << "GLP_EBOUND: " << GLP_EBOUND << "\n"
                  << "GLP_EFAIL: " << GLP_EFAIL << "\n"
                  << "GLP_EOBJLL: " << GLP_EOBJLL << "\n"
                  << "GLP_EOBJUL: " << GLP_EOBJUL << "\n"
                  << "GLP_EITLIM: " << GLP_EITLIM << "\n"
                  << "GLP_ETMLIM: " << GLP_ETMLIM << "\n"
                  << "GLP_ENOPFS: " << GLP_ENOPFS << "\n"
                  << "GLP_ENODFS: " << GLP_ENODFS << "\n";

        exit(code);
    }

    std::ofstream ofs(outputSolutionFile);
    int numCols = glp_get_num_cols(P);
    for(int i=1;i<=numCols;++i)
    {
        ofs << glp_get_col_name(P,i) << " " << glp_get_col_prim(P,i) << "\n";
    }

    //glp_print_sol(P,"alok.txt");

    ofs.flush();
    ofs.close();
}