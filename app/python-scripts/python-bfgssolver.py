import sys
import importlib.util
import numpy as np
import matplotlib.pyplot as plt
from cvxopt import matrix, spmatrix, sparse, spdiag, solvers
from scipy.optimize import minimize

M=None

def load_model(modelFilepath):
    global M
    spec = importlib.util.spec_from_file_location("", modelFilepath)
    M = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(M)

def constraint_as_penalization():
    print("Setting consistent constraints as penalization")
    currR=M._C_dim0[0]
    A=[]
    X=[]
    for r,c,v in zip(M._C_dim0,M._C_dim1,M._C_values):
        if r!=currR:
            for a,x in zip(A,X):
                M._U[x] += a**a - 2*M._c[currR]*a

                for a2,x2 in zip(A,X):
                    if x2==x:
                        continue

                    M._S_dim0.append(x)
                    M._S_dim1.append(x2)
                    M._S_values.append(a*a2)

            currR=r
            A.clear()
            X.clear()

        A.append(v)
        X.append(c)

def binary_constraints():
    print("Setting binary constraints as penalization")
    for i in range(M.numVars):
        M._S_dim0.append(i)
        M._S_dim1.append(i)
        M._S_values.append(1000)

        M._U[i]+=-2


def f(X):
    s=0

    for x,u in zip(X,M._U):
        s+=x*u

    for x,y,v in zip(M._S_dim0,M._S_dim1,M._S_values):
        s+=X[x]*X[y]*v

    for x,y,z,v in zip(M._T_dim0,M._T_dim1,M._T_dim2,M._T_values):
        s+=X[x]*X[y]*X[z]*v

    return s

def rebuild_shape(X,outputFilepath,invert):
    print("Rebuild shape")
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
    constraint_as_penalization()
    
    x0 = np.array( [0]*M.numVars )
    print("Start BFGS")
    res = minimize(f, x0, method='BFGS', options={'disp': True})
    
    rebuild_shape(res.x,outputFilepath,invert)


def main():
    if len(sys.argv) < 4:
        print("Usage: modelFilepath outputFilepath invert")
        exit(1)

    modelFilepath=sys.argv[1]
    outputFilepath=sys.argv[2]
    invert= True if sys.argv[3]=="yes" else False

    load_model(modelFilepath)
    solve(outputFilepath,invert)



if __name__=='__main__':
    main()
