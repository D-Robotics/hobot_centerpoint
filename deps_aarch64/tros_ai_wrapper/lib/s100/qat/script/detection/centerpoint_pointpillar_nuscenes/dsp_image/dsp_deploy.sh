#!/bin/bash
# Copyright (c) 2023 Horizon Robotics.All Rights Reserved.
#
# The material in this file is confidential and contains trade secrets
# of Horizon Robotics Inc. This is proprietary information owned by
# Horizon Robotics Inc. No part of this work may be disclosed,
# reproduced, copied, transmitted, or used in any way for any purpose,
# without the express written permission of Horizon Robotics Inc.


set -x

DIR=$(cd "$(dirname "$0")";pwd)

deploy() {
 num=$((1 + $1))
    
 dsp_state_path=/sys/class/remoteproc/remoteproc$num/state

  # check dsp state and stop if online
  state=$(cat $dsp_state_path)
    if [ "$state" != "offline"  ] ; then
        echo stop > $dsp_state_path
    else
        echo "dsp$1 no need stop"
    fi

    sleep 1

    # echo dsp image path to firmware path
    cd $DIR/
    echo -n $(pwd) > /sys/module/firmware_class/parameters/path

    # set firmware name of dsp core
    echo vdsp$1 > /sys/class/remoteproc/remoteproc$num/firmware

    sleep 1

    # start dsp
    echo start > $dsp_state_path

    sleep 1
}

count=$(ls /dev/vdsp* 2>/dev/null | wc -l)

i=0

while [ $i -lt $count ]; do
    deploy $i
    i=$((i + 1))
done
