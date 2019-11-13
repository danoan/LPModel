#!/usr/bin/python3

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

SHAPE_CONFIGURATIONS=["square","triangle"]
GRID_CONFIGURATIONS=[1.0]
OPTWIDTH_CONFIGURATIONS=[0,1]
SQWEIGTH_CONFIGURATIONS=[1.0]
DATAWEIGTH_CONFIGURATIONS=[0.0]
RELAXATION_CONFIGURATIONS=[R_NONE]
LINEARIZATION_CONFIGURATIONS=[L_PIXEL_PAIR,L_PIXEL_LINEL]

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

        outputFolder = "%s/%s/width-%d/" % (baseFolder,shapeName,optWidth)
        gridPath = "%s/grid.obj" % (outputFolder,)
        api.exportGrid(shapePath,gridPath,optWidth=optWidth)

        modelPath="%s/%s" % (outputFolder,"model.py")
        api.exportCvxopt(shapePath,gridPath,optWidth,sqWeight,dataWeight,lin,modelPath)

        solutionPath="%s/%s" % (outputFolder,"solution.pgm")
        api.solveCVXOpt(modelPath,solutionPath,lin,"constrained-convex")


main()