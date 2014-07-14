#!/usr/local/bin/python
import sys
import os
import commands
from shutil import rmtree
import threading

exe='/home/zwang/project/hmm/Debug/hmm'
tasktype=2
rootdir='/home/zwang/project/hmm/exp/big_indep/'
#testpath='/home/zwang/project/hmm/exp/big/s2TrainList.txt'
testpath='/home/zwang/project/hmm/exp/big/indep.txt'
modeldir='/home/zwang/project/hmm/exp/big/eModel'
cmd='%s %d %s %s %s'%(exe,tasktype,rootdir,testpath,modeldir)
os.system(cmd)
