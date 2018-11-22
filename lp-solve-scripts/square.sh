IMAGE_NAME=$1
GRID_STEP=$2

ROOT_PROJECT_FOLDER=/home-local/dantu1/GIT/PhD/LPModel
BIN_FOLDER=$ROOT_PROJECT_FOLDER/cmake-build-debug
SCRIPT_FOLDER=$ROOT_PROJECT_FOLDER/lp-solve-scripts


INPUT_IMAGES=$ROOT_PROJECT_FOLDER/input-images
mkdir -p $INPUT_IMAGES

OUTPUT_FOLDER=$ROOT_PROJECT_FOLDER/output/$IMAGE_NAME/h$GRID_STEP
mkdir -p $OUTPUT_FOLDER




$BIN_FOLDER/generate-initial-shapes $INPUT_IMAGES $GRID_STEP


$BIN_FOLDER/export-lp $INPUT_IMAGES/$IMAGE_NAME.pgm $OUTPUT_FOLDER
glpsol --cpxlp $OUTPUT_FOLDER/lp-program.txt --tmlim 20 -o $OUTPUT_FOLDER/glpk-out.txt

sh $SCRIPT_FOLDER/read-glpk-output.sh $OUTPUT_FOLDER/glpk-out.txt $OUTPUT_FOLDER/var-assignment.txt

$BIN_FOLDER/generate-solution $OUTPUT_FOLDER/prm.pgm $OUTPUT_FOLDER/var-assignment.txt $OUTPUT_FOLDER/solution.pgm

rm $OUTPUT_FOLDER/prm.pgm
rm $OUTPUT_FOLDER/grid.obj
