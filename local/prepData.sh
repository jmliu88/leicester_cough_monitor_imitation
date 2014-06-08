#!/bin/sh
mfcc='/home/zwang/data/hmm_mfcc'
data='/home/zwang/data/leaveout'
lab=$data/splitedLabs
if [ ! -d $data ];then
    mkdir -p $data
fi
cd $data
cat /dev/null > mfcclist.txt
cat /dev/null > labeledlist.txt
cat /dev/null > emptylist.txt
if [ ! -d $lab ];then
    mkdir -p $lab
fi
pid=$(ls -1 $mfcc)
for x in ${pid[@]};do
    cat $mfcc/$x/mfcclist.txt >> mfcclist.txt
    cat $mfcc/$x/labeledlist.txt >> labeledlist.txt
    cat $mfcc/$x/emptylist.txt >> emptylist.txt
    cp $mfcc/$x/splitedLabs/*  $lab
done

