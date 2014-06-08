#!/bin/sh
if [ ! -d data ];then
    mkdir -p data
fi
ls -1 ~/data/hmm_mfcc|cut -d'/' -f 6 > data/pid.list

