from cvxopt import spmatrix,matrix
from skimage.io import imsave
import numpy as np

def extendSparse(M1,M2,numVars):
    #Append two sparse matrices. They should be disjoint, clearly.

    nr,nc = M1.size
    Avalues  = [ v for v in M1.V ]
    Arows = [ r for r in M1.I ]
    Acols = [ c for c in M1.J ]

    nr = nr+M2.size[0]
    Avalues.extend( [v for v in M2.V] )
    Arows.extend( [r for r in M2.I] )
    Acols.extend( [c for c in M2.J] )

    return spmatrix( Avalues,Arows,Acols,(nr,numVars),'d')


def extendMatrix(M1,M2):

    Avalues = [ x for x in M1 ]
    Avalues.extend( [x for x in M2])

    return matrix(Avalues,(len(Avalues),1),'d')

def constructPGMFromDigitalModel(outputPath,dm):
    img = np.array( dm,dtype="uint8" )
    imsave(outputPath,img)