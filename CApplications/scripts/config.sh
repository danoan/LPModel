#!/usr/bin/env bash

PROJECT_FOLDER=/home-local/dantu1/GIT/PhD/LPModel

SOLVE=$PROJECT_FOLDER/CApplications/scripts/gurobi/lp-solve.sh

R_NONE=none                     #No relaxation
R_ORIGINAL=original             #Relaxe original variables
R_AUXILIAR=auxiliar             #Relaxe auxiliar variables
R_ALL=all                       #Relaxe all variables

L_NONE=none                     #No linearization
L_PIXEL_PAIR=pixel-pair         #Linearize pixel pair second order terms
L_PIXEL_LINEL=pixel-linel       #Linearize pixel linel second order terms
L_ALL_COUPLED=all-coupled       #Linearize all variables with coupling
L_ALL_UNCOUPLED=all-uncoupled   #Linearize all variables with no coupling