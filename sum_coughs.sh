#!/bin/sh
sum=0
for fullfile in `cat exp/4h/s2TestList.txt`; do
filename=$(basename "$fullfile")
extension="${filename##*.}"
filename="${filename%.*}"
labPath=4h/splitedLabs/"$filename".lab

for l in `cat $labPath | awk -F "\t" '{print $3}'`; do
#echo $l
if [ $l == cough ];then
sum=$((sum+1))
fi
done
#echo `cat $labPath | awk -F "\t" '{print $3}'`

done
echo $sum
