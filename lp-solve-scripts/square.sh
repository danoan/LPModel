ROOT_PROJECT_FOLDER=/home-local/dantu1/GIT/PhD/LPModel
BIN_FOLDER=$ROOT_PROJECT_FOLDER/cmake-build-debug
SCRIPT_FOLDER=$ROOT_PROJECT_FOLDER/lp-solve-scripts

INPUT_IMAGES=$ROOT_PROJECT_FOLDER/input-images


OUTPUT_FOLDER=$ROOT_PROJECT_FOLDER/output/square
mkdir -p $OUTPUT_FOLDER
mkdir $OUTPUT_FOLDER/glpk-out
mkdir $OUTPUT_FOLDER/var-assignment


$BIN_FOLDER/generate-initial-shapes $INPUT_IMAGES

#**************SQUARE*************

$BIN_FOLDER/export-lp $INPUT_IMAGES/square.pgm $OUTPUT_FOLDER
glpsol --cpxlp $OUTPUT_FOLDER/lp-program.txt -o $OUTPUT_FOLDER/glpk-out/square.txt

sh $SCRIPT_FOLDER/read-glpk-output.sh $OUTPUT_FOLDER/glpk-out/square.txt $OUTPUT_FOLDER/var-assignment/square.txt

$BIN_FOLDER/generate-solution $OUTPUT_FOLDER/prm.pgm $OUTPUT_FOLDER/var-assignment/square.txt $OUTPUT_FOLDER/solution.pgm

rm grid.obj
rm prm.pgm
