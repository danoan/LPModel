#!/usr/bin/sh

PROJECT_FOLDER=/home-local/dantu1/GIT/PhD/LPModel

SOLVE=$PROJECT_FOLDER/lp-solve-scripts/gurobi/lp-solve.sh

R0=0    #Relaxation level 0
R1=1    #Relaxation level 1
R2=2    #Relaxation level 2

#Ordered by running time

sh $SOLVE square 1.0 0 1.0 0 $R2
sh $SOLVE square 1.0 1 1.0 0 $R2

sh $SOLVE square 1.0 0 1.0 0 $R1

sh $SOLVE square 1.0 0 1.0 0 $R0
