import numpy as np
from cvxopt import matrix, spmatrix, sparse, spdiag
from models.data import DPixelLinel as D
from models.dataCheck import memoryCheck, dataStats
from models.utils import extendSparse, extendMatrix

U = matrix( D._U, (D.numVars,1),'d' )
V = matrix( D._V, (D.numVars,1),'d' )
P = spmatrix( D._P_values, D._P_dim0,D._P_dim1, (D.numVars,D.numVars),'d' )
Z = spmatrix( D._Z_values, D._Z_dim0,D._Z_dim1, (D.numSlackVars,D.numVars),'d' )
z = matrix( D._z, (D.numSlackVars,1),'d' )
C = spmatrix( D._C_values, D._C_dim0,D._C_dim1, (D.numEdges//2,D.numVars),'d' )
c = matrix( D._c, (D.numEdges//2,1),'d' )

dataStats(D.numPixels,D.numEdges,D.numPixelLinelPairs,D.numSlackVars,D.numVars)
memoryCheck( [U,V,P,Z,z,C,c] )

def grad(x):
    return U + V + P*x + 2*Z.T*(Z*x-z) + 2*C.T*(C*x-c)

def grad2(x):
    return (P+P.T) + 2*Z.T*Z + 2*C.T*C

def f(x):
    return U.T*x + V.T*x + x.T*P*x + np.linalg.norm(Z*x-z,2) + np.linalg.norm(C*x-c,2)

def nonLinearConvexModel():
    return (D.numVars,f,grad,grad2)

def constrainedQuadraticModel():
    Q = P.T
    p = U + V

    #Variables are between zero and one
    G = sparse( [ spdiag( [-1 for r in range(D.numVars)] ), spdiag( [1 for r in range(D.numVars)]  ) ] )
    h = matrix( [ [0.0]*D.numVars,[1.0]*D.numVars ], (2*D.numVars,1), 'd' )

    A = extendSparse(Z,C,D.numVars)
    b = extendMatrix(z,c)

    return (Q,p,G,h,A,b)

def penaltyQuadraticModel():
    Q = P.T + Z.T*Z +  C.T*C
    p = U + V - 2*Z.T*z - 2*C.T*c

    #Variables are between zero and one
    G = sparse( [ spdiag( [-1 for r in range(D.numVars)] ), spdiag( [1 for r in range(D.numVars)]  ) ] )
    h = matrix( [ [0.0]*D.numVars,[1.0]*D.numVars ], (2*D.numVars,1), 'd' )

    A=None
    b=None

    return (Q,p,G,h,A,b)


if __name__=='__main__':
    print("Test module: ", f( matrix( [1]*D.numVars, (D.numVars,1),'d' ) ) )