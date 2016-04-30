#!/bin/bash
LIST_TYPE="LRU_ACTIVE_ANON
LRU_INACTIVE_ANON
LRU_ACTIVE_FILE
LRU_INACTIVE_FILE"
rm filtered_*
filters=("Total number of pages"
"Unreferenced Clean"
"Referenced Clean"
"Unreferenced Dirty"
"Referenced Dirty")
for i in {1..12}
do
for list in $LIST_TYPE
do
    echo $list >> filtered_report_${i}
    for((index=0; index < ${#filters[@]#}; index++))
    do
        echo ${filters[index]} >> filtered_report_${i}
        grep -A5 $list report_$i | grep "${filters[index]}" >> filtered_report_${i}
    done
done
done
