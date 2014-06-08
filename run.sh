#!/usr/local/bin/python
import sys
import os
import commands
from shutil import rmtree
import threading

exe='/home/zwang/project/hmm/Debug/hmm'
tasktype=1
rootdir='/home/zwang/project/hmm/exp/small'
datadir='/home/zwang/data/leaveout'
fillerexp=7
cmd='%s %d %s %s %d'%(exe,tasktype,rootdir,datadir,fillerexp)
os.system(cmd)
