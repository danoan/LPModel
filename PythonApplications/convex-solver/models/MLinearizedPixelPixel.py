import numpy as np
from cvxopt import matrix, spmatrix, sparse, spdiag
from models.data import DPixelPair as D
from models.dataCheck import memoryCheck, dataStats
from models.utils import extendSparse, extendMatrix

U = matrix( D._U, (D.numVars,1),'d' )
P1 = matrix( D._P1, (D.numVars,D.numVars),'d' )
P2 = matrix( D._P2, (D.numVars,D.numVars),'d' )
P = P1+P2
Z = spmatrix( D._Z_values, D._Z_row,D._Z_col, (D.numSlackVars,D.numVars),'d' )
z = matrix( D._z, (D.numSlackVars,1),'d' )
C = spmatrix( D._C_values, D._C_row,D._C_col, (D.numEdges//2,D.numVars),'d' )
c = matrix( D._c, (D.numEdges//2,1),'d' )

dataStats(D.numPixels,D.numEdges,D.numPixelPairs,D.numSlackVars,D.numVars)
memoryCheck( [U,P1,P2,P,Z,z,C,c] )

def grad(x):
    return U + P*x + 2*Z.T*(Z*x-z) + 2*C.T*(C*x-c)

def grad2(x):
    return (P+P.T) + 2*Z.T*Z + 2*C.T*C

def f(x):
    return U.T*x + x.T*P*x + np.linalg.norm(Z*x-z,2) + np.linalg.norm(C*x-c,2)

def nonLinearConvexModel():
    return (D.numVars,f,grad,grad2)

def constrainedQuadraticModel():
    Q = P.T
    p = U

    #Variables are between zero and one
    G = sparse( [ spdiag( [-1 for r in range(D.numVars)] ), spdiag( [1 for r in range(D.numVars)]  ) ] )
    h = matrix( [ [0.0]*D.numVars,[1.0]*D.numVars ], (2*D.numVars,1), 'd' )

    A = extendSparse(Z,C,D.numVars)
    b = extendMatrix(z,c)

    return (Q,p,G,h,A,b)

def penaltyQuadraticModel():
    Q = P.T + Z.T*Z +  C.T*C
    p = U - 2*Z.T*z - 2*C.T*c

    #Variables are between zero and one
    G = sparse( [ spdiag( [-1 for r in range(D.numVars)] ), spdiag( [1 for r in range(D.numVars)]  ) ] )
    h = matrix( [ [0.0]*D.numVars,[1.0]*D.numVars ], (2*D.numVars,1), 'd' )

    A=None
    b=None

    return (Q,p,G,h,A,b)

if __name__=='__main__':
    print("Test module: ", f( matrix( [1]*D.numVars, (D.numVars,1),'d' ) ) )