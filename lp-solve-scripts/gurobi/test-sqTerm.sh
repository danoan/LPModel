ROOT_PROJECT_FOLDER=/home-local/dantu1/GIT/PhD/LPModel

LP_SOLVE_APP=$ROOT_PROJECT_FOLDER/lp-solve-scripts/gurobi/lp-solve.sh
TEST_LEVELS_APP=$ROOT_PROJECT_FOLDER/cmake-build-debug/lab/test-levels

OUTPUT_PREFIX=$ROOT_PROJECT_FOLDER/output/lab/test-levels
mkdir -p $OUTPUT_PREFIX

sh $LP_SOLVE_APP square 1 1 1.0 0.0 $OUTPUT_PREFIX/l1
sh $LP_SOLVE_APP square 1 2 1.0 0.0 $OUTPUT_PREFIX/l2
sh $LP_SOLVE_APP square 1 3 1.0 0.0 $OUTPUT_PREFIX/l3

$TEST_LEVELS_APP $OUTPUT_PREFIX

#rm -r -f $OUTPUT_PREFIX/l1 $OUTPUT_PREFIX/l2 $OUTPUT_PREFIX/l3
