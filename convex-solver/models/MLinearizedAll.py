import os

from models.data import DLinearizedAll as D
from models.dataCheck import memoryCheck,dataStats
from models.utils import extendSparse,extendMatrix,constructPGMFromDigitalModel

import numpy as np
from cvxopt import matrix,spmatrix,sparse,spdiag


U = matrix( D._U, (D.numVars,1),'d' )
V = matrix( D._V, (D.numVars,1),'d' )
Z = spmatrix( D._Z_values, D._Z_row,D._Z_col, (D.numSlackVars,D.numVars),'d' )
z = matrix( D._z, (D.numSlackVars,1),'d' )
C = spmatrix( D._C_values, D._C_row,D._C_col, (D.numEdges//2,D.numVars),'d' )
c = matrix( D._c, (D.numEdges//2,1),'d' )

dataStats(D.numPixels,D.numEdges,D.numPixelPairs,D.numSlackVars,D.numVars)
memoryCheck( [U,V,Z,z,C,c] )

def grad(x):
    return U + V + 2*Z.T*(Z*x-z) + 2*C.T*(C*x-c)

def grad2(x):
    return 2*Z.T*Z + 2*C.T*C

def f(x):
    return U.T*x + V.T*x + np.linalg.norm(Z*x-z,2) + np.linalg.norm(C*x-c,2)

def nonLinearConvexModel():
    return (D.numVars,f,grad,grad2)

def constrainedQuadraticModel():
    Q = None
    p = U + V

    #Variables are between zero and one
    G = sparse( [ spdiag( [-1 for r in range(D.numVars)] ), spdiag( [1 for r in range(D.numVars)]  ) ] )
    h = matrix( [ [0.0]*D.numVars,[1.0]*D.numVars ], (2*D.numVars,1), 'd' )

    A = extendSparse(Z,C,D.numVars)
    b = extendMatrix(z,c)

    return (Q,p,G,h,A,b)


def penaltyQuadraticModel():
    Q = Z.T*Z + C.T*C               #Squared penalization terms
    p = U + V - 2*Z.T*z - 2*C.T*c   #Energy unary terms (U,V) + penalization linear terms(Z.T*z,C.T*c)

    #Variables are between zero and one
    G = sparse( [ spdiag( [-1 for r in range(D.numVars)] ), spdiag( [1 for r in range(D.numVars)]  ) ] )
    h = matrix( [ [0.0]*D.numVars,[1.0]*D.numVars ], (2*D.numVars,1), 'd' )

    A=None
    b=None

    return (Q,p,G,h,A,b)

def build_solution(x):
    outputFolder = os.getcwd() + "/output"

    dm = D.digitalModel.copy()
    s=""
    i=0
    for k,v in D.pixelMap.items():
        r,c = v['row'],v['col']

        if x[k]>0.99:
            dm[r][c] = 255
        else:
            dm[r][c] = 0

        s+="x%d:%.10f\n" % (i,x[k])
        i+=1

    constructPGMFromDigitalModel(outputFolder+"/solution.pgm",dm)
    with open(outputFolder+"/solution.txt",'w') as f:
        f.write(s)


if __name__=='__main__':
    print("Test module: ", f( matrix( [1]*D.numVars, (D.numVars,1),'d' ) ) )