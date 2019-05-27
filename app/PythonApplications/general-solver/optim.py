import numpy as np
from scipy.optimize import minimize
from scipy.sparse import coo_matrix as spmatrix

import matplotlib.pyplot as plt

from models.data import DThirdOrder as D
#from models.data import DToy as D

def constraint_as_penalization():
    currR=D._C_dim0[0]
    A=[]
    X=[]
    for r,c,v in zip(D._C_dim0,D._C_dim1,D._C_values):
        if r!=currR:
            for a,x in zip(A,X):
                D._U[x] += a**a - 2*D._c[currR]*a

                for a2,x2 in zip(A,X):
                    if x2==x:
                        continue

                    D._S_dim0.append(x)
                    D._S_dim1.append(x2)
                    D._S_values.append(2*a*a2)

            currR=r
            A.clear()
            X.clear()

        A.append(v)
        X.append(c)


def f(X):
    s=0

    for x,u in zip(X,D._U):
        s+=x*u

    for x,y,v in zip(D._S_dim0,D._S_dim1,D._S_values):
        s+=X[x]*X[y]*v

    for x,y,z,v in zip(D._T_dim0,D._T_dim1,D._T_dim2,D._T_values):
        s+=X[x]*X[y]*X[z]*v

    return s



def main():
    constraint_as_penalization()
    x0 = np.array( [0]*D.numVars )
    res = minimize(f, x0, method='BFGS', options={'disp': True})

    sol = D.digitalModel
    for i,x in enumerate(D.pixelMap.keys()):
        row,col = D.pixelMap[i]['row'],D.pixelMap[i]['col']

        print(res.x[x])

        if res.x[x]>=0.1:
            sol[row][col] = 255
        else:
            sol[row][col] = 0

    aaa = np.array( sol, dtype=np.uint8 )
    plt.imsave("test.png",aaa)

    print(res.x)



if __name__=='__main__':
    main()