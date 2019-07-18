#!/bin/bash

eval $(ps -aux --sort=-%cpu -h | awk -v num=0 \
        '{ if ($3 < 50) {exit}  else {num++;printf("cpupid["num"]=%d;", $2)} } END {printf("cpupid=%d",  $2)}'\
    )








if [[ ${cpunum} -gt 0 ]];then
    count=0;
    for i in ${cpupid[*]}; do
        cout=$[ ${cout} + 1 ]
        eval $(ps -aux -q $i -h | awk -v num=${count})\
        '{if ($3 < 50)} {exit} else {printf("cpupname["num"]=%s;cpupid["num"]=%d;
        cpupuser["num""]=%s;mempcpu["num"]=%.2f;mempmem["num"]=%.2f;", $11, $2, $1, $3, $4)} }\
        END {printf()}'
