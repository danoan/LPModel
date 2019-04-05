#!/sur/bin/python3

import subprocess,sys

PROJECT_FOLDER="/home-local/dantu1/GIT/PhD/LPModel"
BIN_FOLDER="%s/%s" % (PROJECT_FOLDER,"cmake-build-debug/CApplications")


R_NONE="none"                     #No relaxation
R_ORIGINAL="original"             #Relaxe original variables
R_AUXILIAR="auxiliar"            #Relaxe auxiliar variables
R_ALL="all"                       #Relaxe all variables

L_NONE="none"                     #No linearization
L_PIXEL_PAIR="pixel-pair"         #Linearize pixel pair second order terms
L_PIXEL_LINEL="pixel-linel"       #Linearize pixel linel second order terms
L_ALL_COUPLED="all-coupled"       #Linearize all variables with coupling
L_ALL_UNCOUPLED="all-uncoupled"   #Linearize all variables with no coupling

SHAPE_CONFIGURATIONS=["square", "flower"]
GRID_CONFIGURATIONS=[1.0]
OPTWIDTH_CONFIGURATIONS=[0,1,2]
SQWEIGTH_CONFIGURATIONS=[1.0]
DATAWEIGTH_CONFIGURATIONS=[0.0]
RELAXATION_CONFIGURATIONS=[R_NONE, R_ORIGINAL, R_AUXILIAR, R_ALL]
LINEARIZATION_CONFIGURATIONS=[L_ALL_COUPLED, L_ALL_UNCOUPLED]


CONFIG_LIST=[ (SHAPE_CONFIGURATIONS,"shape"),
              (GRID_CONFIGURATIONS,"grid"),
              (OPTWIDTH_CONFIGURATIONS,"optWidth"),
              (SQWEIGTH_CONFIGURATIONS,"sqWeight"),
              (DATAWEIGTH_CONFIGURATIONS,"dataWeight"),
              (RELAXATION_CONFIGURATIONS,"relaxationLevel"),
              (LINEARIZATION_CONFIGURATIONS,'linearizationLevel')]

def rec_combinations(maxList,curr,combList):

    if len(maxList)==0:
        combList.append(curr)
        return

    for el in range(0,maxList[0]):
        rec_combinations(maxList[1:],curr+[el],combList)

def combinations(configList):
    numParams = len(configList)
    maxList = [ len(el) for el,_ in configList ]

    combList=[]
    rec_combinations(maxList,[],combList)

    for c in combList:
        yield tuple( configList[i][0][c[i]] for i in range(numParams) )


def generate_shapes(outputPath,gridStep):
    #USAGE: generate-shapes OUTPUT-PATH GRID-STEP

    print("-------------Generating Shapes-------------")
    print("OUTPUT_PATH: %s\nGridStep: %.2f" % (outputPath,gridStep))

    binary = "%s/%s" % (BIN_FOLDER,"generate-shapes/generate-shapes")
    subprocess.call( [binary,outputPath,str(gridStep)] )

def export_lp(shape,outputFolder,optWidth=1,sqWeight=1,dataWeight=0,rel="none",lin="all-coupled"):
    """
    USAGE: pgm-input-image output-path
    [-w Optimization region width. Default: 1]
    [-s Squared curvature term weight. Default: 1]
    [-d Data term weight. Default: 0]
    [-l Linearization level {none,pixel-pair,pixel-linel,all-coupled,all-uncoupled}. Default: all-coupled
    [-r Relaxation level {none,original,auxiliar,all}. Default: none
    """

    outputPath = "%s/%s/width-%d/rel-%s/lin-%s" % (outputFolder,shape,optWidth,rel,lin)

    print("-------------Export LP-------------")
    binary = "%s/%s" % (BIN_FOLDER,"export-cplex-lp/export-cplex-lp")
    subprocess.call( [binary,shape,outputPath,
                      "%s%s" % ("-w",optWidth),
                      "%s%s" % ("-s",sqWeight),
                      "%s%s" % ("-d",dataWeight),
                      "%s%s" % ("-l",lin),
                      "%s%s" % ("-r","none"),
                      ])


def solve_lp():
    pass

def generate_solution():
    pass


def main():
    argc = len(sys.argv)
    outputPath = "%s/%s" % (PROJECT_FOLDER,"scripts/output/experiment-lp")

    shape_gs_1_output = "%s/%s" % (outputPath,"grid-1.0")
    shape_gs_05_output = "%s/%s" % (outputPath,"grid-0.5")

    generate_shapes(shape_gs_1_output,1.0)
    generate_shapes(shape_gs_05_output,0.5)

    for c in combinations( CONFIG_LIST ):
        print("########### Running instance %s ############" % (c,))
        _shape,gridStep,optWidth,sqWeight,dataWeight,rel,lin = c

        if gridStep==1.0:
            shape = "%s/%s.pgm" % (shape_gs_1_output,_shape)
        elif gridStep==0.5:
            shape = "%s/%s.pgm" % (shape_gs_05_output,_shape)
        else:
            print("Grid step %d not implemented" % (gridStep,))
            exit(1)

        export_lp(shape,outputPath,optWidth,sqWeight,dataWeight,rel,lin)
        exit()



main()
