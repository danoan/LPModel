#!/usr/bin/python3

import model as M
from cvxopt import solvers , matrix
import numpy as np

U = matrix( M._U, (M.numVars,1),'d' )
P1 = matrix( M._P1, (M.numVars,M.numVars),'d' )
P2 = matrix( M._P2, (M.numVars,M.numVars),'d' )
P = P1+P2
Z = matrix( M._Z, (M.numSlackVars,M.numVars),'d' )
z = matrix( M._z, (M.numSlackVars,1),'d' )
C = matrix( M._C, (M.numEdges//2,M.numVars),'d' )
c = matrix( M._c, (M.numEdges//2,1),'d' )

def grad(x):
    return U + P*x + 2*Z.T*(Z*x-z) + 2*C.T*(C*x-c)

def grad2(x):
    return (P+P.T) + 2*Z.T*Z + 2*C.T*C

def f(x):
    return U.T*x + x.T*P*x + np.linalg.norm(Z*x-z,2) + np.linalg.norm(C*x-c,2)


def nonlinear_convex_solver():
       
    def F(xx=None,zz=None):
        if xx is None: return 0, matrix(0.0,(M.numVars,1))
        if zz is None: return f(xx),grad(xx).T

        return f(xx),grad(xx).T,zz[0]*grad2(xx)

    return solvers.cp(F)['x']


def quadratic_solver():
    print( np.all( np.array(P)>=0) )
    Q = P.T + Z.T*Z +  C.T*C
    L = U - 2*Z.T*z - 2*C.T*c

    return solvers.qp(Q,L)['x']

def main():
    nonlinear_convex_solver()
        
                    

if __name__=='__main__':
    main()
