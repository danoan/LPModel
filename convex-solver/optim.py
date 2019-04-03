#!/usr/bin/python3

from cvxopt import solvers, matrix

from models import MLinearizedAll as PO
from models import MLinearizedPixelLinel as PL
from models import MLinearizedPixelPixel as PP


def nonlinear_convex_solver(numVars,f,grad,grad2):
       
    def F(xx=None,zz=None):
        if xx is None: return 0, matrix(0.0,(numVars,1))
        if zz is None: return f(xx),grad(xx).T

        return f(xx),grad(xx).T,zz[0]*grad2(xx)

    return solvers.cp(F)['x']


def quadratic_solver(qm):
    Q,p,G,h,A,b = qm
    return solvers.qp(Q,p,G,h,A,b)['x']


def constrainedConvex():
    print("------------------- Constrained Quadratic Models ---------------------------")
    print("****Model:Linearize PixelPixel")
    try:
        quadratic_solver(PP.constrainedQuadraticModel())
    except ValueError as v:
        print("Error: ",v)
        print("Probably non-semidefinite positive matrix!")


    print("****Model:Linearize PixelLinel")
    try:
        quadratic_solver(PL.constrainedQuadraticModel())
    except ValueError as v:
        print("Error: ",v)
        print("Probably non-semidefinite positive matrix!")

    print("****Model:Linearize All")
    try:
        quadratic_solver(PO.constrainedQuadraticModel())
    except ValueError as v:
        print("Error: ",v)
        print("Probably non-semidefinite positive matrix!")



def penaltyConvex():
    print("------------------- Penalty Quadratic Models ---------------------------")
    print("****Model:Linearize PixelPixel")
    try:
        quadratic_solver(PP.penaltyQuadraticModel())
    except ValueError as v:
        print("Error: ",v)
        print("Probably non-semidefinite positive matrix!")


    print("****Model:Linearize PixelLinel")
    try:
        quadratic_solver(PL.penaltyQuadraticModel())
    except ValueError as v:
        print("Error: ",v)
        print("Probably non-semidefinite positive matrix!")

    print("****Model:Linearize All")
    try:
        x = quadratic_solver(PO.penaltyQuadraticModel())
        PO.build_solution(x)
    except ValueError as v:
        print("Error: ",v)
        print("Probably non-semidefinite positive matrix!")


def main():
    constrainedConvex()
    penaltyConvex()



if __name__=='__main__':
    main()
