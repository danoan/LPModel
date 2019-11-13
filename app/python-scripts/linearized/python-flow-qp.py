#!/usr/bin/python3

import subprocess,sys,os

PROJECT_FOLDER=None

def clean_folder(outputFolder):
    subprocess.call( ["mogrify -format pgm %s/*.png" % (outputFolder,)],shell=True )
    subprocess.call( ["rm %s/*.png" % (outputFolder,)],shell=True)


def generate_shape(di):
    bin = "%s/%s" % (PROJECT_FOLDER,"cmake-build-debug/app/CApplications/generate-shapes/generate-shapes")
    subprocess.call( [bin,
                      di["output-path"],
                      di["shape"],
                      di["grid-step"],
                      ])

def export_grid(di):
    bin = "%s/%s" % (PROJECT_FOLDER,"cmake-build-debug/app/CApplications/export-grid/export-grid")
    subprocess.call( [bin,
                      "%s%s" % ("-w",di["w"]),
                      di["pgm-shape-image"],
                      di["output-path"]
                      ])

def python_writer(di):
    bin = "%s/%s" % (PROJECT_FOLDER,"cmake-build-debug/app/CApplications/export-cvxopt/export-cvxopt")
    subprocess.call( [bin,
                      "%s%s" % ("-w",di["w"]),
                      "%s%s" % ("-s",di["s"]),
                      "%s%s" % ("-d",di["d"]),
                      "%s%s" % ("-l",di["l"]),
                      di["pgm-input-image"],
                      di["grid-object-file"],
                      di["output-path"],
                      ])

def python_qp_solver(di):
    pythonSource = "%s/%s" % (PROJECT_FOLDER,"app/python-scripts/linearized/python-qpsolver.py")
    subprocess.call( ["python3",
                      pythonSource,
                      di["model-filepath"],
                      di["output-path"],
                      di["lmode"],
                      di["smode"]
                      ])

def build_instance(shapeImage,outputFolder,prefix,lmode,smode):
    outputGridObject = "%s/%s/%s" % (outputFolder,prefix,"grid.obj")

    diEG = {"w":"1",
            "pgm-shape-image":shapeImage,
            "output-path":outputGridObject}
    export_grid(diEG)

    outputModel = "%s/%s/%s" % (outputFolder,prefix,"model.py")
    diPW = {"w":"1",
            "s":"1.0",
            "d":"0.0",
            "l":lmode,
            "pgm-input-image":shapeImage,
            "grid-object-file":outputGridObject,
            "output-path": outputModel}
    python_writer(diPW)

    outputQP = "%s/%s/%s" % (outputFolder,prefix,"solution.png")
    diQP={"model-filepath":diPW["output-path"],
           "output-path":outputQP,
           "lmode":lmode,
           "smode":smode
           }
    python_qp_solver(diQP)

def main():
    if len(sys.argv)<8:
        print("Usage: projectFolder shapeStr gridStep iterations outputFolder linearizationMode solverMode")
        exit(1)

    global PROJECT_FOLDER

    PROJECT_FOLDER = sys.argv[1]
    shapeStr = sys.argv[2]
    gridStep = sys.argv[3]
    iterations = int(sys.argv[4])
    outputFolder = sys.argv[5]
    lmode = sys.argv[6]
    smode = sys.argv[7]

    outputShapeImage = "%s/%s/%s" % (outputFolder,"0","solution.pgm")
    diGS = {"output-path":outputShapeImage,
            "shape":shapeStr,
            "grid-step":gridStep}
    generate_shape(diGS)

    for i in range(iterations):
        outputShapeImage = "%s/%d/%s" % (outputFolder,i,"solution.pgm")
        build_instance(outputShapeImage,outputFolder,str(i+1),lmode,smode)
        clean_folder("%s/%d" % (outputFolder,i+1) )


main()