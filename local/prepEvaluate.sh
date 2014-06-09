#!/bin/sh
if [ $# -lt 1 ];then
    echo '1 param'
    exit 0
fi
if [ ! -e evaluate ];then
    mkdir -p evaluate
fi
cat $1 |sed 's/.*\/\(.*\).mfcc/\1/g'>evaluate/test.list
