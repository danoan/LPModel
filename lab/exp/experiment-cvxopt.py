#!/usr/bin/python3

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

SHAPE_CONFIGURATIONS=["square"]
GRID_CONFIGURATIONS=[1.0]
OPTWIDTH_CONFIGURATIONS=[0,1,2]
SQWEIGTH_CONFIGURATIONS=[1.0]
DATAWEIGTH_CONFIGURATIONS=[0.0]
RELAXATION_CONFIGURATIONS=[R_NONE]
LINEARIZATION_CONFIGURATIONS=[L_PIXEL_PAIR,L_PIXEL_LINEL]


CONFIG_LIST=[ (SHAPE_CONFIGURATIONS,"shape"),
              (GRID_CONFIGURATIONS,"grid"),
              (OPTWIDTH_CONFIGURATIONS,"optWidth"),
              (SQWEIGTH_CONFIGURATIONS,"sqWeight"),
              (DATAWEIGTH_CONFIGURATIONS,"dataWeight"),
              (RELAXATION_CONFIGURATIONS,"relaxationLevel")]

PYTHON_CONTAINER = "%s/%s" % (PROJECT_FOLDER,"PythonApplications/convex-solver")


def createPythonContainer(pythonContainerSource,pythonContainerTarget):
    print("\n-------------Creating Python Container -------------\n")

    subprocess.call(["mkdir",
                     "-p",
                     pythonContainerTarget
                     ])

    subprocess.call(["cp",
                     "-r",
                     pythonContainerSource,
                     pythonContainerTarget])

    return "%s/%s" % (pythonContainerTarget,os.path.basename(pythonContainerSource))


def exportCvxopt(pgmInputImage,pythonContainer,optWidth,sqWeight,dataWeight,lin):
    """
    Usage: pgm-input-image output-path
    [-w Optimization region width. Default: 1]
    [-s Squared curvature term weight. Default: 1]
    [-d Data term weight. Default: 0]
    [-l Linearization level {none,pixel-pair,pixel-linel,all-coupled,all-uncoupled}. Default: all-coupled]
    """
    print("\n-------------Exporting CvxOpt Python Models-------------\n")

    cleansedName = "".join( [ s.title() for s in lin.split("-") ] )
    outputPath = "%s/%s/D%s.py" % (pythonContainer,"models/data",cleansedName)

    binary = "%s/%s" % (BIN_FOLDER,"export-cvxopt/export-cvxopt")
    subprocess.call( [binary,
                      pgmInputImage,
                      outputPath,
                      "%s%s" % ("-w",optWidth),
                      "%s%s" % ("-s",sqWeight),
                      "%s%s" % ("-d",dataWeight),
                      "%s%s" % ("-l",lin)
                      ])


def cvxOpt(pythonContainer,logFilePath):

    logFile = open(logFilePath,'w')
    pythonSource = "%s/%s" % (pythonContainer,"optim.py")
    subprocess.call( ["python3",
                      pythonSource
                      ],
                     stdout=logFile)




def main():
    baseFolder = "%s/%s" % (PROJECT_FOLDER,"CApplications/scripts/output/experiment-cvxopt")

    shape_gs_1_output = "%s/%s" % (baseFolder,"shapes/grid-1.0")

    shapes = Shapes()
    shapes.generate(shape_gs_1_output,1.0)

    for c in combinations( CONFIG_LIST ):
        print("\n########### Running instance %s ############\n" % (c,))
        shapeName,gridStep,optWidth,sqWeight,dataWeight,rel = c

        shapePath = shapes.get(gridStep,shapeName)
        outputFolder = "%s/%s/width-%d/" % (baseFolder,shapeName,optWidth)

        pythonContainer = createPythonContainer(PYTHON_CONTAINER,outputFolder)
        exportCvxopt(shapePath,pythonContainer,optWidth,sqWeight,dataWeight,L_PIXEL_LINEL)
        exportCvxopt(shapePath,pythonContainer,optWidth,sqWeight,dataWeight,L_PIXEL_PAIR)

        logFilePath = "%s/%s" % (outputFolder,"log.txt")
        cvxOpt(pythonContainer,logFilePath)


main()