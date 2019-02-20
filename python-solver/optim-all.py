#!/usr/bin/python3

from cvxopt import solvers , matrix, spmatrix, spdiag, sparse
import numpy as np


import model_all as M
U = matrix( M._U, (M.numVars,1),'d' )
V = matrix( M._V, (M.numVars,1),'d' )
Z = spmatrix( M._Z_values, M._Z_row,M._Z_col, (M.numSlackVars,M.numVars),'d' )
z = matrix( M._z, (M.numSlackVars,1),'d' )
C = spmatrix( M._C_values, M._C_row,M._C_col, (M.numEdges//2,M.numVars),'d' )
c = matrix( M._c, (M.numEdges//2,1),'d' )

def grad(x):
    return U + V + 2*Z.T*(Z*x-z) + 2*C.T*(C*x-c)

def grad2(x):
    return 2*Z.T*Z + 2*C.T*C

def f(x):
    return U.T*x + V.T*x + np.linalg.norm(Z*x-z,2) + np.linalg.norm(C*x-c,2)


def nonlinear_convex_solver():

    def F(xx=None,zz=None):
        if xx is None: return 0, matrix(0.0,(M.numVars,1))
        if zz is None: return f(xx),grad(xx).T

        return f(xx),grad(xx).T,zz[0]*grad2(xx)

    return solvers.cp(F)['x']


def quadratic_solver():
    P = Z.T*Z + C.T*C
    q = U + V - 2*Z.T*z - 2*C.T*c

    #Variables are between zero and one
    G = sparse( [ spdiag( [-1 for r in range(M.numVars)] ), spdiag( [1 for r in range(M.numVars)]  ) ] )
    h = matrix( [ [0.0]*M.numVars,[1.0]*M.numVars ], (2*M.numVars,1), 'd' )

    return solvers.qp(P,q,G,h)


def main():
    #nonlinear_convex_solver()
    sol = quadratic_solver()


    with open("qp.sol",'w') as f:
        for i,x in enumerate(sol['x']):
            f.write( "%d: %.2f\n" % (i,x) )



if __name__=='__main__':
    main()
