import sys
import importlib.util
import numpy as np
import matplotlib.pyplot as plt
from cvxopt import matrix, spmatrix, sparse, spdiag, solvers

M=None

def load_model(modelFilepath):
    global M
    spec = importlib.util.spec_from_file_location("", modelFilepath)
    M = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(M)

def f(x,U,P):
    s= M.constantFactor*(U.trans()*x + 0.5*x.trans()*P*x ) + M.constantTerm
    return s[0]


def round_down(X,U,P):
    Nx=matrix(X)
    for i in range(M.numVars):
        X0 = matrix(Nx)
        X1 = matrix(Nx)

        X0[i]=0
        X1[i]=1

        dx0 = f(X0,U,P)
        dx1 = f(X1,U,P)
        dx=(dx1-dx0)

        if dx < 0:
            Nx[i]=1
        else:
            Nx[i]=0

    return Nx

def rebuild_shape(X,outputFilepath,invert):
    digShape = M.digitalModel
    rows=len(digShape)-1

    for i,x in enumerate(M.pixelMap.keys()):
        row,col = M.pixelMap[x]['row'],M.pixelMap[x]['col']

        if X[x]==1.0:
            digShape[rows-row][col] = 0 if invert else 255
        else:
            digShape[rows-row][col] = 255 if invert else 0

    aaa = np.array( digShape, dtype=np.uint8 )
    plt.imsave(outputFilepath,aaa,cmap='gray')


def solve(outputFilepath,invert):
    U = matrix( M._U, (M.numVars,1),'d' )
    P = spmatrix( M._P_values, M._P_dim0,M._P_dim1, (M.numVars,M.numVars),'d' )

    G0 = sparse( spdiag( [-1 for r in range(M.numVars)] ) )
    G1 = sparse( spdiag( [1 for r in range(M.numVars)] ) )
    G = matrix([G0,G1])
    h = matrix( [ [0.0]*M.numVars,[1.0]*M.numVars ], (2*M.numVars,1), 'd' )


    X=solvers.qp(P,U,G,h)['x']
    NX=round_down(X,U,P)
    rebuild_shape(NX,outputFilepath,invert)

    print(f(X,U,P))
    print(f(NX,U,P))

def main():
    if len(sys.argv) < 4:
        print("Usage: modelFilepath outputFilepath invert")
        exit(1)

    modelFilepath=sys.argv[1]
    outputFilepath=sys.argv[2]
    invert= True if sys.argv[3]=="yes" else False

    load_model((modelFilepath))
    solve(outputFilepath,invert)


main()