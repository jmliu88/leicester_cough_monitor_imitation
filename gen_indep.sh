#!/bin/sh
li=$(cat /home/zwang/data/all.list)
tar='indep.list'
cat /dev/null > $tar
for x in ${li[@]};do
    cat /home/zwang/data/hmm_mfcc/$x/mfcclist.txt >> $tar
done
