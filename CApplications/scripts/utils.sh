#!/usr/bin/env bash

get_from_list()
{
    INDEX=$1
    shift

    RET=""

    i=0
    for el in $@
    do
        if [ "${i}" -eq "${INDEX}" ]
        then
            RET=$el
        fi

        i=`expr $i + 1`
    done;

    echo $RET
}

count()
{
    i=0
    for el in $1
    do
        i=`expr $i + 1`
    done
    echo $i
}

rec_combinations()
{
    local DEPTH=$1
    shift

    if [ "$DEPTH" -eq "0" ]
    then
        echo ${@}
    else
        local M=$1
        shift

        local i=0
        while [ "$i" -lt "$M" ]
        do
            rec_combinations $(($DEPTH - 1)) $@ $i
            i=`expr $i + 1`
        done
    fi

}

combinations()
{
    pairs=$#
    s=""
    while [ "$#" -gt "0" ]
    do
        el=`count "$1"`
        s="$s $el"
        shift
    done;

    echo `rec_combinations $pairs $s`

}