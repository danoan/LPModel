import numpy as np
from cvxopt import matrix, spmatrix, sparse, spdiag
from . dataCheck import memoryCheck, dataStats
from . utils import extendSparse, extendMatrix

class PixelPixel:
    def __init__(self,M):
        self.numVars = M.numVars
        self.U = matrix( M._U, (M.numVars,1),'d' )
        self.P1 = spmatrix( M._P1_values, M._P1_dim0, M._P1_dim1, (M.numVars,M.numVars),'d' )
        self.P2 = spmatrix( M._P2_values, M._P2_dim0, M._P2_dim1, (M.numVars,M.numVars),'d' )
        self.P = self.P1+self.P2
        self.Z = spmatrix( M._Z_values, M._Z_dim0,M._Z_dim1, (M.numSlackVars,M.numVars),'d' )
        self.z = matrix( M._z, (M.numSlackVars,1),'d' )
        self.C = spmatrix( M._C_values, M._C_dim0,M._C_dim1, (M.numEdges//2,M.numVars),'d' )
        self.c = matrix( M._c, (M.numEdges//2,1),'d' )

        dataStats(M.numPixels,M.numEdges,M.numPixelPairs,M.numSlackVars,M.numVars)
        memoryCheck( [self.U,self.P1,self.P2,self.P,self.Z,self.z,self.C,self.c] )

    def grad(self,x):
        return self.U + self.P*x + 2*self.Z.T*(self.Z*x-self.z) + 2*self.C.T*(self.C*x-self.c)
    
    def grad2(self,x):
        return (self.P+self.P.T) + 2*self.Z.T*self.Z + 2*self.C.T*self.C
    
    def f(self,x):
        return self.U.T*x + x.T*self.P*x + np.linalg.norm(self.Z*x-z,2) + np.linalg.norm(self.C*x-c,2)
    
    def nonLinearConvexModel(self):
        return (self.numVars,self.f,self.grad,self.grad2)
    
    def constrainedQuadraticModel(self):
        Q = self.P.T
        p = self.U
    
        #Variables are between zero and one
        G = sparse( [ spdiag( [-1 for r in range(self.numVars)] ), spdiag( [1 for r in range(self.numVars)]  ) ] )
        h = matrix( [ [0.0]*self.numVars,[1.0]*self.numVars ], (2*self.numVars,1), 'd' )
    
        A = extendSparse(self.Z,self.C,self.numVars)
        b = extendMatrix(self.z,self.c)
    
        return (Q,p,G,h,A,b)
    
    def penaltyQuadraticModel(self):
        Q = self.P.T + self.Z.T*self.Z +  self.C.T*self.C
        p = self.U - 2*self.Z.T*self.z - 2*self.C.T*self.c
    
        #Variables are between zero and one
        G = sparse( [ spdiag( [-1 for r in range(self.numVars)] ), spdiag( [1 for r in range(self.numVars)]  ) ] )
        h = matrix( [ [0.0]*self.numVars,[1.0]*self.numVars ], (2*self.numVars,1), 'd' )
    
        A=None
        b=None
    
        return (Q,p,G,h,A,b)

    def __str__(self):
        return "pixel-pair"