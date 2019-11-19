#!/usr/bin/env bash

SCRIPT_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
PROJECT_PATH=$( cd $SCRIPT_PATH && cd ../../ && pwd )

SUMMARY_APP=$1

function collect
{
    INPUT_FOLDER=$1
    FLOW_FOLDER=$INPUT_FOLDER/flow
    mkdir -p $FLOW_FOLDER

    for d in $( ls $INPUT_FOLDER )
    do
        if [ $d != "flow" ]
        then
            n=$( echo $d | cut -d"t" -f2 )
            cp $INPUT_FOLDER/$d/solution.pgm $FLOW_FOLDER/$n.pgm
        fi

    done

    $SUMMARY_APP -j1 $FLOW_FOLDER $INPUT_FOLDER/summary.eps
}

collect $PROJECT_PATH/lab/exp/output/lp-g05/flower/grid-1.00/width-0/all/all-coupled
collect $PROJECT_PATH/lab/exp/output/lp-g05/flower/grid-0.50/width-0/all/all-coupled

collect $PROJECT_PATH/lab/exp/output/lp-g05/square/grid-1.00/width-0/all/all-coupled
collect $PROJECT_PATH/lab/exp/output/lp-g05/square/grid-0.50/width-0/all/all-coupled

collect $PROJECT_PATH/lab/exp/output/lp-g05/triangle/grid-1.00/width-0/all/all-coupled
collect $PROJECT_PATH/lab/exp/output/lp-g05/triangle/grid-0.50/width-0/all/all-coupled





