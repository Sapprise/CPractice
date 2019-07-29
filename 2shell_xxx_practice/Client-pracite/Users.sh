#!/bin/bash

NowTime=`date +%Y-%m-%d__%H:%M:%S`

UsrNumber1=`cat /etc/passwd | cut -d ":" -f 1 | wc -l`
UsrNumber=0
for (( i = 1; i <= ${UsrNumber1}; i++ )); do
    Uid=`cat /etc/passwd | cut -d ":" -f 3 | head -n ${i} | tail -n 1`
    if [[ ${Uid} -gt 500 ]]; then
        UsrNumber=$[ ${UsrNumber} + 1 ]
    fi

    if [[ ${Uid} -eq 0 ]]; then
        Name_Root=`cat /etc/passwd | cut -d ":" -f 1 | head -n ${i} | tail -n 1`
        if [[ ${i} -eq 1 ]]; then
        RootName=${Name_Root}
        else    
        RootName=${RootName},${Name_Root}
        fi
    fi
done


InNumber=`last | wc -l`
if [[ ${InNumber} -ge 5 ]]; then
    InNumber=3;
else
    InNumber=$[ ${InNumber} - 2 ]
fi
for (( j = 1; j <= ${InNumber}; j++ )); do
    Name_IN=`last | cut -d " " -f 1 | head -n ${j} | tail -n 1`
    if [[ ${j} -eq 1 ]]; then
        InName=${Name_IN}
    else
        InName=${InName},${Name_IN}
    fi
done

NowInNumber=`w | wc -l`
for ((k = 3; k <= ${NowInNumber}; k++)); do
    NowUsrName=`w | cut -d " " -f 1 | head -n ${k} | tail -n 1`
    NowUsrIP=`w | tr -s " " | head -n ${k} | tail -n 1 | cut -d " " -f 3`
    NowUsrTTY=`w | tr -s " " | head -n ${k} | tail -n 1 | cut -d " " -f 2`
    if [[ ${k} -eq 3 ]]; then
        NowUsrInfor=${NowUsrName}__${NowUsrIP}__${NowUsrTTY}
    else
        NowUsrInfor=${NowUsrInfor},${NowUsrName}__${NowUsrIP}__${NowUsrTTY}
    fi
done

echo "${NowTime} ${UsrNumber} [${InName}] [${RootName}] [${NowUsrInfor}]"
