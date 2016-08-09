#!/usr/local/bin/python
import sys
import os
import commands
from shutil import rmtree
import threading

exe='/home/james/hmm/Debug/hmm'
tasktype=1
rootdir='/home/james/hmm/exp/big'
datadir='/home/james/hmm/leaveout'
fillerexp=7
cmd='%s %d %s %s %d'%(exe,tasktype,rootdir,datadir,fillerexp)
os.system(cmd)
