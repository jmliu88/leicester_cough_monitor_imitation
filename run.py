#!/usr/local/bin/python
import sys
import os
import commands
from shutil import rmtree
import threading
from JM_util import SendEmail


topic = 'HMM training started'
content = 'good luck'
SendEmail.send_notification(topic,content)

exe='/home/james/hmm/Debug/hmm'
tasktype=1
rootdir='/home/james/hmm/exp/4h_new'
datadir='/home/james/hmm/4h_new'
#rootdir='/home/james/hmm/exp/big'
#datadir='/home/james/hmm/leaveout'
fillerexp=7
cmd='%s %d %s %s %d'%(exe,tasktype,rootdir,datadir,fillerexp)
r = os.system(cmd)
print r

if r == 0:
    topic = 'HMM training Done'
    content = 'good luck'
    SendEmail.send_notification(topic,content)
else:
    topic = 'ERROR in training HMM'
    content = 'good luck'
    SendEmail.send_notification(topic,content)

