#!/bin/bash

NowTime=`date +%Y-%m-%d__%H:%M:%S`

HostName=`hostname`

OS=`cat /etc/issue | head -n 1`

Kernel=`uname -r`

NumUseTime=`uptime | tr -s " " "\n" | wc -l`

if [[ ${NumUseTime} -eq 11 ]]; then
    UseTime=`uptime | tr -s ":," " " | awk '{printf("%s_0_day,_%s_hours,_%s_minutes %s %s %s", $4, $5, $6, $11, $12, $13)}'`
else 
    UseTime=`uptime | tr -s ":," " " | awk '{printf("%s_%s_day,_%s_hours,_%s_minutes %s %s %s", $4, $5, $7, $8, $13, $14, $15)}'`
fi

Disknums=`df -l | cut -d " " -f 1 | grep /dev | wc -l`
Total[0]=0
Residual[0]=0
for (( i = 1; i <= ${Disknums}; i++));do
    Disk[${i}]=`df -l | cut -d " " -f 1 | grep /dev | head -n ${i} | tail -n 1`
    Total[${i}]=`df | grep ${Disk[${i}]} | tr -s " " | cut -d " " -f 2`
    Residual[${i}]=`df | grep ${Disk[${i}]} | tr -s " " | cut -d " " -f 4`

    Total[0]=$[ ${Total[0]} + ${Total[${i}]} ]
    Residual[0]=$[ ${Residual[0]} + ${Residual[${i}]} ]
done
Occupancy=$[ 100 - ${Residual[0]}*100 / ${Total[0]} ]

MemBig=`free -m | head -n 2 | tail -n 1 | awk '{printf("%d", $2)}'`
MemUse=`free -m | head -n 2 | tail -n 1 | awk '{printf("%d", $3)}'`
Mem=$[ ${MemUse}*100 / ${MemBig} ]

Tt=`echo "scale=2;$(cat /sys/class/thermal/thermal_zone0/temp) / 1000" | bc`

if [[ ${Occupancy} > 90 ]]; then
    Oc_jigao="warning"
elif [[  ${Tt} > 50 ]]; then
    Oc_jigao="note"
else
    Oc_jigao="normal"
fi

if [[ ${Mem} > 80 ]]; then
    Mem_jigao="warning"
elif [[  ${Tt} > 50 ]]; then
    Mem_jigao="note"
else
    Mem_jigao="normal"
fi

if [[ ${Tt} > 70 ]]; then
    Tt_jigao="warning"
elif [[  ${Tt} > 50 ]]; then
    Tt_jigao="note"
else
    Tt_jigao="normal"
fi

echo "${NowTime} ${HostName} ${OS} ${Kernel} ${NumUseTime} ${UseTime} ${Total} ${Occupancy}% ${MemBig} ${Mem}% ${Tt} ${Oc_jigao} ${Mem_jigao} ${Tt_jigao}"

