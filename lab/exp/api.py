import subprocess,sys,os

if 'GUROBI_HOME' in os.environ.keys():
    GUROBI_HOME=os.environ['GUROBI_HOME']
else:
    print("GUROBI_HOME is not defined.")
    exit()

class API:

    def __init__(self,project_folder,rel_bin_folder,rel_script_folder):
        self.project_folder = project_folder
        self.bin_folder =  "%s/%s" % (self.project_folder,rel_bin_folder)
        self.script_folder = "%s/%s" % (self.project_folder,rel_script_folder)

    def generateShape(self,shapeName,gridStep,outputFilepath):
        #USAGE: generate-shapes OUTPUT-PATH GRID-STEP

        print("\n-------------Generating Shapes-------------\n")
        print("OUTPUT_PATH: %s\nGridStep: %.2f" % (outputFilepath,gridStep))

        binary = "%s/%s" % (self.bin_folder,"app/CApplications/generate-shapes/generate-shapes")
        subprocess.call( [binary,
                          outputFilepath,
                          shapeName,
                          str(gridStep)] )


    def exportODRModel(self,pgmShapeImage,outputFilepath,optWidth):
        """
        Usage: PGMShapeImage OutputFilePath
        [-w OptRegionWidth default: 1]";
        """
        print("\n-------------Exporting ODR Model-------------\n")

        binary = "%s/%s" % (self.bin_folder,"app/CApplications/create-ODR-image/create-ODR-image")
        subprocess.call( [binary,
                          pgmShapeImage,
                          outputFilepath,
                          "%s %s" % ("-w",optWidth)
                          ] )


    def exportGrid(self,pgmShapeImage,outputFilepath,optWidth=1):
        """
        Usage: pgmShapeImage OutputPath
        [-o Optimization region width. Default: 1]"
        """
        print("\n-------------Exporting Grid Object-------------\n")
        binary = "%s/%s" % (self.bin_folder,"app/CApplications/export-grid/export-grid")
        subprocess.call( [binary,
                          pgmShapeImage,
                          outputFilepath,
                          "%s %d" % ("-w",optWidth)
                          ])


    def exportLP(self,shapePath,gridObjectFile,outputFilepath,optWidth=1,sqWeight=1,dataWeight=0,rel="none",lin="all-coupled"):
        """
        USAGE: pgm-input-image output-path
        [-w Optimization region width. Default: 1]
        [-s Squared curvature term weight. Default: 1]
        [-d Data term weight. Default: 0]
        [-l Linearization level {none,pixel-pair,pixel-linel,all-coupled,all-uncoupled}. Default: all-coupled
        [-r Relaxation level {none,original,auxiliar,all}. Default: none
        """

        print("\n-------------Exporting LP-------------\n")
        binary = "%s/%s" % (self.bin_folder,"app/CApplications/export-cplex-lp/export-cplex-lp")
        subprocess.call( [binary,shapePath,gridObjectFile,outputFilepath,
                          "%s%s" % ("-w",optWidth),
                          "%s%s" % ("-s",sqWeight),
                          "%s%s" % ("-d",dataWeight),
                          "%s%s" % ("-l",lin),
                          "%s%s" % ("-r",rel),
                          ])



    def exportQP(self,shapePath,gridObjectFile,outputFilepath,optWidth=1,sqWeight=1,dataWeight=0,rel="none",lin="all-coupled"):
        """
        USAGE: pgm-input-image output-path
        [-w Optimization region width. Default: 1]
        [-s Squared curvature term weight. Default: 1]
        [-d Data term weight. Default: 0]
        [-l Linearization level {none,pixel-pair,pixel-linel,all-coupled,all-uncoupled}. Default: all-coupled
        [-r Relaxation level {none,original,auxiliar,all}. Default: none
        """
        print("\n-------------Exporting QP-------------\n")
        binary = "%s/%s" % (self.bin_folder,"app/CApplications/export-cplex-qp/export-cplex-qp")
        subprocess.call( [binary,shapePath,gridObjectFile,outputFilepath,
                          "%s%s" % ("-w",optWidth),
                          "%s%s" % ("-s",sqWeight),
                          "%s%s" % ("-d",dataWeight),
                          "%s%s" % ("-l",lin),
                          "%s%s" % ("-r","none"),
                          ])



    def solveLP(self,lpCplexFile,outputFilepath,logFilepath,method=1):
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

        binary = "%s/%s" % (GUROBI_HOME, "bin/gurobi_cl")
        subprocess.call( [binary,
                          "%s=%d" %("Method",method),
                          "%s=%s" %("ResultFile",outputFilepath),
                          "%s=%s" %("LogFile",logFilepath),
                          lpCplexFile
                          ])



    def solveQP(self,lpCplexFile,outputFilepath,method=1):
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
        binary = "%s/%s" % (GUROBI_HOME, "bin/gurobi_cl")
        subprocess.call( [binary,
                          "%s=%d" %("Method",method),
                          "%s=%s" %("ResultFile",outputFilepath),
                          lpCplexFile
                          ])

    def solveCVXOpt(self,modelFilepath,outputFilepath,lmode,smode):
        print("\n-------------Solving CVXOpt -------------\n")

        subprocess.call( ["python3",
                          "%s/%s" % (self.script_folder,"linearized/python-qpsolver.py"),
                          modelFilepath,
                          outputFilepath,
                          lmode,
                          smode
                          ])

    def exportCvxopt(self,pgmInputImage,gridObjectFilepath,optWidth,sqWeight,dataWeight,lin,outputFilepath):
        """
        Usage: pgm-input-image grid-object-filepath output-path
        [-w Optimization region width. Default: 1]
        [-s Squared curvature term weight. Default: 1]
        [-d Data term weight. Default: 0]
        [-l Linearization level {none,pixel-pair,pixel-linel,all-coupled,all-uncoupled}. Default: all-coupled]
        """
        print("\n-------------Exporting CvxOpt Python Models-------------\n")
        binary = "%s/%s" % (self.bin_folder,"app/CApplications/export-cvxopt/export-cvxopt")
        subprocess.call( [binary,
                          pgmInputImage,
                          gridObjectFilepath,
                          outputFilepath,
                          "%s%s" % ("-w",optWidth),
                          "%s%s" % ("-s",sqWeight),
                          "%s%s" % ("-d",dataWeight),
                          "%s%s" % ("-l",lin)
                          ])

    def generateSolution(self,shapePath, optWidth, gridObjectFile, lpSolutionFile, outputFilepath):
        """
        USAGE: shapePath optWidth gridObjectFile lpCplexFile outputFilePath
        """

        print("\n-------------Generating Solution-------------\n")

        binary = "%s/%s" % (self.bin_folder,"app/CApplications/generate-solution/generate-solution")
        subprocess.call( [binary,
                          shapePath,
                          "%d" % (optWidth,),
                          gridObjectFile,
                          lpSolutionFile,
                          outputFilepath])


    def iterationZero(self,outputFolder,shapePath,optWidth):
        iterationFolder = "%s/%s" % (outputFolder,"it0")

        subprocess.call( ["mkdir",
                          "-p",
                          iterationFolder
                          ])

        subprocess.call( ["cp",
                          shapePath,
                          "%s/%s" % (iterationFolder,"solution.pgm")
                          ])

        self.exportODRModel(shapePath,iterationFolder+"/odr.eps",optWidth)