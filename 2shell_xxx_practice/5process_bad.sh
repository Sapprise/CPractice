#!/bin/bash

Max_N=50
NowTime=`date +%Y-%m-%d__%H:%M:%S`

Mem="ps -aux --sort=%mem | tail -n 1"
Cpu="ps -aux --sort=%cpu | tail -n 1"

PID1=`eval $Mem | tr -s " " | cut -d " " -f 2`
Memmax1=`eval $Mem | tr -s " " |cut -d " " -f 4`

PID2=`eval $Cpu | tr -s " " | cut -d " " -f 2`
Cpumax2=`eval $Cpu | tr -s " " | cut -d " " -f 3`

MEMpid=0
CPUpid=0
if [[ ${Memmax1} > ${Max_N} ]]; then
    MEMpid=${PID1}
fi
if [[ ${Cpumax2} > ${Max_N} ]]; then
    CPUpid=${PID2}
fi

if [[ ${MEMpid} -gt 0 && ${CPUpid} -gt 0 ]]; then
    sleep 5
else 
    exit 0
fi
if [[ ${MEMpid} > 0 ]]; then
    Memmax3=`ps -aux | grep ${MEMpid} | head -n 1 | tr -s " " | cut -d " " -f 4`
    Cpumax3=`ps -aux | grep ${MEMpid} | head -n 1 | tr -s " " | cut -d " " -f 3`
    if [[ ${Memmax3} > ${Max_N} || ${Cpumax3} > ${Max_N} ]]; then
        Process=`ps -aux | grep ${MEMpid} | head -n 1 | tr -s " " | cut -d " " -f 11`
        User=`ps -aux | grep ${MEMpid} | head -n 1 | tr -s " " | cut -d " " -f 1`
        echo "${NowTime} ${Process} ${MEMpid} ${User} ${Cpumax3} ${Memmax3}"
    fi
fi
if [[ ${CPUpid} -gt 0  ]]; then
    Memmax4=`ps -aux | grep ${CPUpid} | head -n 1 | tr -s " " | cut -d " " -f 4`
    Cpumax4=`ps -aux | grep ${CPUpid} | head -n 1 | tr -s " " | cut -d " " -f 3`
    if [[ ${Memmax4} > ${Max_N} || ${Cpumax4} > ${Max_N} ]]; then
        Process=`ps -aux | grep ${CPUpid} | head -n 1 | tr -s " " | cut -d " " -f 11`
        User=`ps -aux | grep ${CPUpid} | head -n 1 | tr -s " " | cut -d " " -f 1`
        echo "${NowTime} ${Process} ${CPUpid} ${User} ${Cpumax4} ${Memmax4}"
    fi
fi 

