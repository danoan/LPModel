import sys
import importlib.util
import numpy as np
import matplotlib.pyplot as plt
from cvxopt import matrix, solvers
from model.PixelLinel import PixelLinel
from model.PixelPixel import PixelPixel

M=None

def load_model(modelFilepath):
    global M
    spec = importlib.util.spec_from_file_location("", modelFilepath)
    M = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(M)

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


def quadratic_solver(qm):
    Q,p,G,h,A,b = qm
    return solvers.qp(Q,p,G,h,A,b)['x']


def constrainedConvex(model):
    print("------------------- Constrained Quadratic Models ---------------------------")
    print("****Model:Linearize %s" % (model,))
    try:
        quadratic_solver(model.constrainedQuadraticModel())
    except ValueError as v:
        print("Error: ",v)
        print("Probably non-semidefinite positive matrix!")



def penaltyConvex(model):
    print("------------------- Penalty Quadratic Models ---------------------------")
    print("****Model:Linearize %s" % (model,))
    try:
        quadratic_solver(model.penaltyQuadraticModel())
    except ValueError as v:
        print("Error: ",v)
        print("Probably non-semidefinite positive matrix!")


def main():
    if len(sys.argv) < 5:
        print("Usage: modelFilepath outputFilepath lmode smode")
        exit(1)

    modelFilepath=sys.argv[1]
    outputFilepath=sys.argv[2]
    lmode = sys.argv[3]
    smode = sys.argv[4]

    load_model((modelFilepath))

    model=None
    print(lmode)
    if lmode=="pixel-linel":
        model = PixelLinel(M)
    elif lmode=="pixel-pair":
        model = PixelPixel(M)
    else:
        raise("Linearization mode not recognized. Must be pixel-linel or pixel-pixel")

    if smode=="constrained-convex":
        constrainedConvex(model)
    elif smode=="penalty-convex":
        penaltyConvex(model)
    else:
        raise("Resolution mode not recognized. Must be constrained-covex or penalty-convex")



if __name__=='__main__':
    main()
