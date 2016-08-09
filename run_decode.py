#!/usr/local/bin/python
import sys
import os
import commands
from shutil import rmtree
import threading
from JM_util import SendEmail


topic = 'HMM decoding started'
content = 'good luck'
SendEmail.send_notification(topic,content)

exe='/home/james/hmm/Debug/hmm'
tasktype=2
rootdir='/home/james/hmm/exp/4h'
datadir='/home/james/hmm/4h'
#rootdir='/home/james/hmm/exp/big'
#datadir='/home/james/hmm/leaveout'
fillerexp=7
## rootdir,mfcclist,modelDir(which contains eMM.mmf, eMMf2.mmf)
cmd='%s %d %s %s %s'%(exe,tasktype,rootdir,rootdir+'/s2TestList.txt',rootdir+'/eModel')
#cmd='%s %d %s %s %s'%(exe,tasktype,rootdir,rootdir+'/s1fTrainList.txt',rootdir+'/eModel')
r = os.system(cmd)
print r

if r == 0:
    topic = 'HMM decoding Done'
    content = 'good luck'
    SendEmail.send_notification(topic,content)
else:
    topic = 'ERROR in decoding HMM'
    content = 'good luck'
    SendEmail.send_notification(topic,content)

