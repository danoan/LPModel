#!/sur/bin/python3

import subprocess,sys,os
from api import *
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

SHAPE_CONFIGURATIONS=["square", "triangle", "flower","bean"]
GRID_CONFIGURATIONS=[1.0,0.5]
OPTWIDTH_CONFIGURATIONS=[0]
SQWEIGTH_CONFIGURATIONS=[1.0]
DATAWEIGTH_CONFIGURATIONS=[0.0]
#RELAXATION_CONFIGURATIONS=[R_ALL, R_ORIGINAL, R_AUXILIAR, R_NONE]
RELAXATION_CONFIGURATIONS=[R_ALL]
LINEARIZATION_CONFIGURATIONS=[L_ALL_COUPLED]
MAX_ITERATIONS=20


CONFIG_LIST=[ (RELAXATION_CONFIGURATIONS,"relaxationLevel"),
              (LINEARIZATION_CONFIGURATIONS,'linearizationLevel'),
              (OPTWIDTH_CONFIGURATIONS,"optWidth"),
              (GRID_CONFIGURATIONS,"grid"),
              (SHAPE_CONFIGURATIONS,"shape"),
              (SQWEIGTH_CONFIGURATIONS,"sqWeight"),
              (DATAWEIGTH_CONFIGURATIONS,"dataWeight")]

def main():
    argc = len(sys.argv)

    if argc < 5:
        raise("Usage: projectFolder relBinFolder relScriptFolder outputFolder")

    projectFolder = sys.argv[1]
    relBinFolder = sys.argv[2]
    relScriptFolder = sys.argv[3]
    baseFolder = sys.argv[4]

    outputShapes = "%s/%s" % (baseFolder,"shapes")
    api = API(projectFolder,relBinFolder,relScriptFolder)
    for c in combinations( CONFIG_LIST ):
        print("\n########### Running instance %s ############\n" % (c,))
        rel,lin,optWidth,gridStep,shapeName,sqWeight,dataWeight = c

        shapePath = "%s/%.2f/%s.pgm" % (outputShapes,gridStep,shapeName)
        api.generateShape(shapeName,gridStep,shapePath)

        outputFolder = "%s/%s/grid-%.2f/width-%d/%s/%s" % (baseFolder,shapeName,gridStep,optWidth,rel,lin)
        api.iterationZero(outputFolder,shapePath,optWidth)

        it=1
        while it<=MAX_ITERATIONS:
            iterationFolder = "%s/it%04d" % (outputFolder,it)

            gridPath = "%s/grid.obj" % (iterationFolder,)
            api.exportGrid(shapePath,gridPath,optWidth=optWidth)

            lpPath = "%s/formulation.lp" % (iterationFolder,)
            api.exportLP(shapePath,gridPath,lpPath,optWidth,sqWeight,dataWeight,rel,lin)

            solutionPath="%s/formulation.sol" % (iterationFolder,)
            logPath="%s/formulation.log" % (iterationFolder,)
            api.solveLP(lpPath,solutionPath,logFilepath=logPath)

            shapeSolutionFile = "%s/solution.pgm" % (iterationFolder,)
            api.generateSolution(shapePath, optWidth, gridPath, solutionPath,shapeSolutionFile)

            energyValuePath = "%s/energyValue.txt" % (iterationFolder,)
            api.evaluateEnergy(shapePath,shapeSolutionFile,optWidth,gridPath,energyValuePath)

            shapePath = shapeSolutionFile

            it+=1



main()
