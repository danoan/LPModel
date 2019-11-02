#!/sur/bin/python3

import subprocess,sys,os
from utils import *

R_NONE="none"                     #No relaxation
R_ORIGINAL="original"             #Relaxe original variables
R_AUXILIAR="auxiliar"            #Relaxe auxiliar variables
R_ALL="all"                       #Relaxe all variables

L_NONE="none"                     #No linearization
L_PIXEL_PAIR="pixel-pair"         #Linearize pixel pair second order terms
L_PIXEL_LINEL="pixel-linel"       #Linearize pixel linel second order terms
L_ALL_COUPLED="all-coupled"       #Linearize all variables with coupling
L_ALL_UNCOUPLED="all-uncoupled"   #Linearize all variables with no coupling

SHAPE_CONFIGURATIONS=["square", "triangle"]
GRID_CONFIGURATIONS=[1.0]
OPTWIDTH_CONFIGURATIONS=[0,1]
SQWEIGTH_CONFIGURATIONS=[1.0]
DATAWEIGTH_CONFIGURATIONS=[0.0]
RELAXATION_CONFIGURATIONS=[R_ALL, R_ORIGINAL, R_AUXILIAR, R_NONE]
LINEARIZATION_CONFIGURATIONS=[L_ALL_COUPLED]
MAX_ITERATIONS=5


CONFIG_LIST=[ (SHAPE_CONFIGURATIONS,"shape"),
              (GRID_CONFIGURATIONS,"grid"),
              (OPTWIDTH_CONFIGURATIONS,"optWidth"),
              (SQWEIGTH_CONFIGURATIONS,"sqWeight"),
              (DATAWEIGTH_CONFIGURATIONS,"dataWeight"),
              (RELAXATION_CONFIGURATIONS,"relaxationLevel"),
              (LINEARIZATION_CONFIGURATIONS,'linearizationLevel')]


def main():
    argc = len(sys.argv)
    baseFolder = "%s/%s" % (PROJECT_FOLDER,"lab/exp/output/experiment-lp")

    shape_gs_1_output = "%s/%s" % (baseFolder,"shapes/grid-1.0")

    shapes = Shapes()
    shapes.generate(shape_gs_1_output,1.0)

    for c in combinations( CONFIG_LIST ):
        print("\n########### Running instance %s ############\n" % (c,))
        shapeName,gridStep,optWidth,sqWeight,dataWeight,rel,lin = c

        shapePath = shapes.get(gridStep,shapeName)
        outputFolder = resolveOutputFolder(baseFolder,shapeName,optWidth,rel,lin)

        iterationZero(outputFolder,shapePath,optWidth)
        it=1
        while it<=MAX_ITERATIONS:
            iterationFolder = "%s/it%d" % (outputFolder,it)

            gridObjectFile = exportGrid(shapePath,iterationFolder,optWidth)
            lpCplexFile = exportLP(shapeName,shapePath,gridObjectFile,iterationFolder,optWidth,sqWeight,dataWeight,rel,lin)
            lpSolutionFile = solveLP(lpCplexFile,iterationFolder)

            shapeSolutionFile = generateSolution(shapePath, optWidth, gridObjectFile, lpSolutionFile,iterationFolder)
            shapePath = shapeSolutionFile

            it+=1



main()