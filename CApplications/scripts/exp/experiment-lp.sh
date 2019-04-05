#!/usr/bin/bash

source ../config.sh
source ../utils.sh

SHAPE_CONFIGURATIONS="square flower"
GRID_CONFIGURATIONS="1.0"
OPTWIDTH_CONFIGURATIONS="0 1 2"
SQWEIGTH_CONFIGURATIONS="1.0"
DATAWEIGTH_CONFIGURATIONS="0.0"
RELAXATION_CONFIGURATIONS="${R_NONE} ${R_ORIGINAL} ${R_AUXILIAR} ${R_ALL}"
LINEARIZATION_CONFIGURATIONS="${L_ALL_COUPLED} ${L_ALL_UNCOUPLED}"



#get_from_list 1 $RELAXATION_CONFIGURATIONS

#indexes=`combinations "${SHAPE_CONFIGURATIONS}" "${OPTWIDTH_CONFIGURATIONS}"`



#for c in {0,1}{3,4,5}
#do
#    echo ${c:0:1}
#done




#Ordered by running time

#shape grid optWidth sqWeight dataWeight RLevel LLevel
#
#sh $SOLVE square 1.0 0 1.0 0 $R2
#sh $SOLVE square 1.0 1 1.0 0 $R2
#
#sh $SOLVE square 1.0 0 1.0 0 $R1
#
#sh $SOLVE square 1.0 0 1.0 0 $R0
