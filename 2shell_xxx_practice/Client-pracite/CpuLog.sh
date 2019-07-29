#!/bin/bash

function _find_temperature {
    Tt=$(echo "scale=2;$(cat /sys/class/thermal/thermal_zone0/temp) / 1000" | bc )
    Tt_other=$(echo "$(cat /sys/class/thermal/thermal_zone0/temp) / 10" | bc )
    jigao=0
    if [[ ${Tt_other} -gt 7000 ]]; then
        jigao="warning"
    elif [[  ${Tt_other} -gt 5000 ]]; then
        jigao="note"
    else
        jigao="normal"
    fi
}

function _load_ {
    load1=$(cat /proc/loadavg | cut -d " " -f 1)
    load2=$(cat /proc/loadavg | cut -d " " -f 2)
    load3=$(cat /proc/loadavg | cut -d " " -f 3)
}

function _now_time_ {
    time=`date +%Y-%m-%d__%H:%M:%S `
}

function _cpu_use_ {
    cpusTotal1=0;
    cpusTotal2=0;

    for (( i = 3; i<=9; i++ ));do
        cpusTotal1=$[$(head -n 1 /proc/stat | cut -d " " -f ${i}) + ${cpusTotal1} ]
    done

    cpuUsed1=${cpusTotal1}
    for (( i = 6; i<=7; i++ ));do
        cpuUsed1=$[ ${cpuUsed1} - $(head -n 1 /proc/stat | cut -d " " -f ${i}) ]
    done

    sleep 0.5

    for (( i = 3; i<=9; i++ ));do
        cpusTotal2=$[$(head -n 1 /proc/stat | cut -d " " -f ${i}) + ${cpusTotal2} ]
    done

    cpuUsed2=${cpusTotal2}
    for (( i = 6; i<=7; i++ ));do
        cpuUsed2=$[ ${cpuUsed2} - $(head -n 1 /proc/stat | cut -d " " -f ${i}) ]
    done

    cpu=$(echo "scale=2;(${cpuUsed2} - ${cpuUsed1}) * 100 / (${cpusTotal2} - ${cpusTotal1})" | bc)
}

_now_time_
_load_
_cpu_use_    
_find_temperature
echo "${time} ${load1} ${load2} ${load3} ${cpu} ${Tt}℃ ${jigao}"
