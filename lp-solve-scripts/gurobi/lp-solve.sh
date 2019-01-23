#!/usr/bin/sh

IMAGE_NAME=$1
GRID_STEP=$2
LEVELS=$3
SQ_WEIGHT=$4
DATA_WEIGHT=$5
RELAXATION_LEVEL=$6  #0 - All binaries; 1 - Relax auxiliary variables ; 2 - Relax all variables

ROOT_PROJECT_FOLDER=/home-local/dantu1/GIT/PhD/LPModel
BIN_FOLDER=$ROOT_PROJECT_FOLDER/cmake-build-debug
SCRIPT_FOLDER=$ROOT_PROJECT_FOLDER/lp-solve-scripts

if [ -z $7 ]
then
    OUTPUT_FOLDER_PREFIX=$IMAGE_NAME/H$GRID_STEP-R$RELAXATION_LEVEL-L$LEVELS
else
    OUTPUT_FOLDER_PREFIX=$7
fi


SHAPES_OUTPUT_FOLDER=$ROOT_PROJECT_FOLDER/output/generate-initial-shapes
mkdir -p $SHAPES_OUTPUT_FOLDER


EXPORT_LP_OUTPUT_FOLDER=$ROOT_PROJECT_FOLDER/output/export-lp/$OUTPUT_FOLDER_PREFIX
mkdir -p $EXPORT_LP_OUTPUT_FOLDER


GUROBI_OUTPUT_FOLDER=$ROOT_PROJECT_FOLDER/output/gurobi/$OUTPUT_FOLDER_PREFIX
mkdir -p $GUROBI_OUTPUT_FOLDER


SOLUTION_OUTPUT_FOLDER=$ROOT_PROJECT_FOLDER/output/solution/$OUTPUT_FOLDER_PREFIX
mkdir -p $SOLUTION_OUTPUT_FOLDER



$BIN_FOLDER/generate-initial-shapes     $SHAPES_OUTPUT_FOLDER \
                                        $GRID_STEP

$BIN_FOLDER/export-lp   $SHAPES_OUTPUT_FOLDER/$IMAGE_NAME.pgm \
                        $LEVELS \
                        $SQ_WEIGHT \
                        $DATA_WEIGHT \
                        $RELAXATION_LEVEL \
                        $EXPORT_LP_OUTPUT_FOLDER


gurobi_cl   Method=1 \
            ResultFile=$GUROBI_OUTPUT_FOLDER/${IMAGE_NAME}.sol \
            $EXPORT_LP_OUTPUT_FOLDER/${IMAGE_NAME}.lp


$BIN_FOLDER/generate-solution   $EXPORT_LP_OUTPUT_FOLDER/prm.pgm \
                                $LEVELS \
                                $EXPORT_LP_OUTPUT_FOLDER/grid.obj \
                                $GUROBI_OUTPUT_FOLDER/${IMAGE_NAME}.sol \
                                $SOLUTION_OUTPUT_FOLDER/solution.pgm
