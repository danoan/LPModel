import subprocess,sys,os

PROJECT_FOLDER="/home/dantu/GIT/PhD/LPModel"
BIN_FOLDER="%s/%s" % (PROJECT_FOLDER,"cmake-build-debug/app/CApplications")
if 'GUROBI_HOME' in os.environ.keys():
    GUROBI_HOME=os.environ['GUROBI_HOME']
else:
    print("GUROBI_HOME is not defined.")
    exit()

def recCombinations(maxList,curr,combList):

    if len(maxList)==0:
        combList.append(curr)
        return

    for el in range(0,maxList[0]):
        recCombinations(maxList[1:],curr+[el],combList)

def combinations(configList):
    numParams = len(configList)
    maxList = [ len(el) for el,_ in configList ]

    combList=[]
    recCombinations(maxList,[],combList)

    for c in combList:
        yield tuple( configList[i][0][c[i]] for i in range(numParams) )

class Shapes:
    def __init__(self):
        self.gridPathDict = {}

    def generate(self,outputPath,gridStep):
        #USAGE: generate-shapes OUTPUT-PATH GRID-STEP

        self.gridPathDict.update( {gridStep:outputPath} )

        print("\n-------------Generating Shapes-------------\n")
        print("OUTPUT_PATH: %s\nGridStep: %.2f" % (outputPath,gridStep))

        binary = "%s/%s" % (BIN_FOLDER,"generate-shapes/generate-shapes")
        subprocess.call( [binary,outputPath,str(gridStep)] )

    def get(self,gridStep,shapeName):
        return "%s/%s.pgm" % (self.gridPathDict[gridStep],shapeName)


def resolveOutputFolder(baseFolder,shapeName,optWidth,rel,lin):
    return "%s/%s/width-%d/rel-%s/lin-%s" % (baseFolder,shapeName,optWidth,rel,lin)


def exportODRModel(pgmShapeImage,outputFolder,optWidth):
    """
    Usage: PGMShapeImage OutputFilePath
    [-w OptRegionWidth default: 1]";
    """
    print("\n-------------Exporting ODR Model-------------\n")

    outputPath = "%s/%s" % (outputFolder,"odrModel.eps")
    binary = "%s/%s" % (BIN_FOLDER,"create-ODR-image/create-ODR-image")
    subprocess.call( [binary,
                      pgmShapeImage,
                      outputPath,
                      "%s %s" % ("-w",optWidth)
                      ] )

    return outputPath


def exportGrid(pgmShapeImage,outputFolder,optWidth=1):
    """
    Usage: pgmShapeImage OutputPath
    [-o Optimization region width. Default: 1]"
    """
    print("\n-------------Exporting Grid Object-------------\n")
    outputPath = "%s/%s" % (outputFolder,"grid.obj")
    binary = "%s/%s" % (BIN_FOLDER,"export-grid/export-grid")
    subprocess.call( [binary,
                      pgmShapeImage,
                      outputPath,
                      "%s %d" % ("-w",optWidth)
                      ])

    return outputPath


def exportLP(shapeName,shapePath,outputFolder,optWidth=1,sqWeight=1,dataWeight=0,rel="none",lin="all-coupled"):
    """
    USAGE: pgm-input-image output-path
    [-w Optimization region width. Default: 1]
    [-s Squared curvature term weight. Default: 1]
    [-d Data term weight. Default: 0]
    [-l Linearization level {none,pixel-pair,pixel-linel,all-coupled,all-uncoupled}. Default: all-coupled
    [-r Relaxation level {none,original,auxiliar,all}. Default: none
    """

    outputPath = "%s/%s" % (outputFolder,"formulation.lp")

    print("\n-------------Exporting LP-------------\n")
    binary = "%s/%s" % (BIN_FOLDER,"export-cplex-lp/export-cplex-lp")
    subprocess.call( [binary,shapePath,outputPath,
                      "%s%s" % ("-w",optWidth),
                      "%s%s" % ("-s",sqWeight),
                      "%s%s" % ("-d",dataWeight),
                      "%s%s" % ("-l",lin),
                      "%s%s" % ("-r",rel),
                      ])

    return outputPath


def exportQP(shapeName,shapePath,outputFolder,optWidth=1,sqWeight=1,dataWeight=0,rel="none",lin="all-coupled"):
    """
    USAGE: pgm-input-image output-path
    [-w Optimization region width. Default: 1]
    [-s Squared curvature term weight. Default: 1]
    [-d Data term weight. Default: 0]
    [-l Linearization level {none,pixel-pair,pixel-linel,all-coupled,all-uncoupled}. Default: all-coupled
    [-r Relaxation level {none,original,auxiliar,all}. Default: none
    """

    outputPath = "%s/%s" % (outputFolder,"formulation.lp")

    print("\n-------------Exporting QP-------------\n")
    binary = "%s/%s" % (BIN_FOLDER,"export-cplex-qp/export-cplex-qp")
    subprocess.call( [binary,shapePath,outputPath,
                      "%s%s" % ("-w",optWidth),
                      "%s%s" % ("-s",sqWeight),
                      "%s%s" % ("-d",dataWeight),
                      "%s%s" % ("-l",lin),
                      "%s%s" % ("-r","none"),
                      ])

    return outputPath


def solveLP(lpCplexFile,outputFolder,method=1):
    """
    USAGE: lpCplexFormulationFile Method=[-1,0,1,2]
    Method: -1=automatic,
            0=primal simplex,
            1=dual simplex, //Use this if memory is a problem
            2=barrier,
            3=concurrent,
            4=deterministic concurrent,
            5=deterministic concurrent simplex
    """
    print("\n-------------Solving LP (with Gurobi)-------------\n")

    outputPath = "%s/%s" % (outputFolder,"lp.sol")
    logOutputPath = "%s/%s" % (outputFolder,"lp.log")

    binary = "%s/%s" % (GUROBI_HOME, "bin/gurobi_cl")
    subprocess.call( [binary,
                      "%s=%d" %("Method",method),
                      "%s=%s" %("ResultFile",outputPath),
                      "%s=%s" %("LogFile",logOutputPath),
                      lpCplexFile
                      ])

    return outputPath


def solveQP(lpCplexFile,outputFolder,method=1):
    """
    USAGE: lpCplexFormulationFile Method=[-1,0,1,2,3,4,5]
    Method: -1=automatic,
            0=primal simplex,
            1=dual simplex,//Use this if memory is a problem
            2=barrier,
            3=concurrent (x),
            4=deterministic concurrent (x),
            5=deterministic concurrent simplex (x)
    """
    print("\n-------------Solving LP (with Gurobi)-------------\n")

    outputPath = "%s/%s" % (outputFolder,"qp.sol")

    binary = "%s/%s" % (GUROBI_HOME, "bin/gurobi_cl")
    subprocess.call( [binary,
                      "%s=%d" %("Method",method),
                      "%s=%s" %("ResultFile",outputPath),
                      lpCplexFile
                      ])

    return outputPath

def generateSolution(shapePath, optWidth, gridObjectFile, lpSolutionFile, outputFolder):
    """
    USAGE: shapePath optWidth gridObjectFile lpCplexFile outputFilePath
    """

    print("\n-------------Generating Solution-------------\n")

    outputPath = "%s/%s" % (outputFolder,"solution.pgm")

    binary = "%s/%s" % (BIN_FOLDER,"generate-solution/generate-solution")
    subprocess.call( [binary,
                      shapePath,
                      "%d" % (optWidth,),
                      gridObjectFile,
                      lpSolutionFile,
                      outputPath])

    return outputPath

def iterationZero(outputFolder,shapePath,optWidth):
    iterationFolder = "%s/%s" % (outputFolder,"it0")

    subprocess.call( ["mkdir",
                      "-p",
                      iterationFolder
                      ])

    subprocess.call( ["cp",
                      shapePath,
                      "%s/%s" % (iterationFolder,"solution.pgm")
                      ])

    exportODRModel(shapePath,iterationFolder,optWidth)