GLPK_OUTPUT=$1
OUTPUT_FILE=$2

START_LINE=`grep "Column name" $GLPK_OUTPUT -n | grep "[0-9]*" -o`
END_LINE=`grep "Karush-Kuhn" $GLPK_OUTPUT -n | grep "[0-9]*" -o`

NUM_LINES=`expr $END_LINE - $START_LINE`

tail $GLPK_OUTPUT -n +$START_LINE | head -n $NUM_LINES | awk '{print $2 "\t" $4}' | grep -oP "(?<=x)[0-9]*" > temp_var_index
tail $GLPK_OUTPUT -n +$START_LINE | head -n $NUM_LINES | awk '{print $2 "\t v" $4}' | grep -oP "(?<=v)[0-9]*(\\.[0-9]*){0,}" > temp_var_value

#grep -c "" temp_var_index
#grep -c "" temp_var_value

paste temp_var_index temp_var_value > var-assignment.txt
echo $NUM_LINES | cat - var-assignment.txt > temp.txt && mv temp.txt $OUTPUT_FILE
rm temp_var_index
rm temp_var_value




