#!/bin/bash

COMMAND='lscpu && echo ----------------------------- && grep . /sys/devices/system/cpu/cpu0/cache/index*/*'

bsub -R "select[model==XeonE3_1585Lv5]" -W 00:01 "echo Euler III XeonE3_1585Lv5  && $COMMAND"
bsub -R "select[model==XeonGold_5118]"  -W 00:01 "echo Euler  V  XeonGold_5118   && $COMMAND"
