from functools import reduce

import numpy as np
from cvxopt import matrix,spmatrix,sparse

def ms(M):
    #Compute matrix storage in bytes

    Sf = float(1.0).__sizeof__()

    if(type(M)==spmatrix):
        return len(M.I)*Sf + len(M.J)*Sf + len(M.V)*Sf
    elif(type(M)==matrix):
        return reduce(lambda x,y:x+Sf*y,M.size)
    else:
        print(type(M))
        exit(1)

def memoryCheck(matrixList,maxAllowed=2000):
    #Check if data surpasses maxAllowed (in Mb) value
    print("****Memory Check****")
    memory_requested = reduce( lambda x,y:x+ms(y),matrixList,0)/pow(1024,2)
    print("Memory requested: %.2f Mb\n" % (memory_requested,))

    if memory_requested > maxAllowed:
        print("!!!!!!Memory requested (%d) is higher than allowed (%d)\n" % (memory_requested,maxAllowed))
        return False

    return True

def dataStats(numPixels,numEdges,numPixelPairs,numSlackVars,numVars):
    print("****Data Stats****")
    print("#Pixels:%d\n#Edges:%d\n#PixelPairs:%d\n#SlackVars:%d\n#TotalVars:%d\n" % (numPixels,numEdges,numPixelPairs,numSlackVars,numVars))