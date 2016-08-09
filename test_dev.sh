#!/usr/local/bin/python
import sys
import os
import commands
from shutil import rmtree
import threading

exe='/home/james/hmm/Debug/hmm'
tasktype=2
#rootdir='/home/james/hmm/exp/big_indep_dev/'
#testpath='/home/james/hmm/exp/big/s2DevList.txt'
#modeldir='/home/james/hmm/exp/big/eModel'
rootdir='/home/james/hmm/exp/big_indep_dev/'
testpath='/home/james/hmm/exp/small/indep.txt'
modeldir='/home/james/hmm/exp/big/eModel'
cmd='%s %d %s %s %s'%(exe,tasktype,rootdir,testpath,modeldir)
os.system(cmd)
