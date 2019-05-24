import numpy as np
from scipy.optimize import minimize
from scipy.sparse import coo_matrix as spmatrix

from models.data import DThirdOrder as D

U = np.matrix( D._U, dtype=np.float64 )
S = spmatrix( (D._S_values, (D._S_dim0,D._S_dim1)), shape=(D.numVars,D.numVars),dtype=np.float64 )
T = spmatrix( (D._T_values, (D._T_dim0,D._T_dim1,D._T_dim2)), shape=(D.numVars,D.numVars,D.numVars),dtype=np.float64 )
C = spmatrix( (D._C_values, (D._C_dim0,D._C_dim1)), shape=(D.numEdges//2,D.numVars),dtype=np.float64 )
c = np.matrix( D._c, dtype=np.float64 )

def f(x):
    pass

def d():
    pass

def main():
    print(len(U),len(V),len(P))
    #res = minimize(rosen, x0, method='BFGS', options={'disp': True})
    pass

if __name__=='__main__':
    main()