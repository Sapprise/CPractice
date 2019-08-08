#!/bin/bash

NowTime=`date +%Y-%m-%d__%H:%M:%S`

Disknums=`df -l | cut -d " " -f 1 | grep /dev | wc -l`
Total[0]=0
Residual[0]=0
for (( i = 1; i <= ${Disknums}; i++));do
    Disk[${i}]=`df -l | cut -d " " -f 1 | grep /dev | head -n ${i} | tail -n 1`
    Total[${i}]=`df | grep ${Disk[${i}]} | tr -s " " | cut -d " " -f 2`
    Residual[${i}]=`df | grep ${Disk[${i}]} | tr -s " " | cut -d " " -f 4`
    Occupancy[${i}]=`df | grep ${Disk[${i}]} | tr -s " " | cut -d " " -f 5`
    Namedisk[${i}]=`df | grep ${Disk[${i}]} | tr -s " " | cut -d " " -f 6`
    echo "${NowTime} 1 ${Namedisk[${i}]} ${Total[${i}]} ${Residual[${i}]}   ${Occupancy[${i}]}" 

    Total[0]=$[ ${Total[0]} + ${Total[${i}]} ]
    Residual[0]=$[ ${Residual[0]} + ${Residual[${i}]} ]
done
Occupancy[0]=$[ 100 - ${Residual[0]}*100 / ${Total[0]} ]
echo "${NowTime} 0 disk ${Total[0]} ${Residual[0]}   ${Occupancy[0]}%"



