#!/usr/local/bin/python
import sys
import os
import commands
from shutil import rmtree
import threading

exe='/home/zwang/project/hmm/Debug/hmm'
tasktype=0
root='/home/zwang/data/htkmfcc'
data=root
audio='/home/zwang/data/audio'
lab='/home/zwang/data/TextGrids'
dmp3=1
wav10dir='/home/zwang/data/wav10s'

class run_hmm(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
    def set_param(self,lis):
        self.lis=lis
    def run(self):
        lis=self.lis
        for itd in lis:
            subPath=audio+'/'+itd
            if os.path.isdir(subPath)==False:
                continue;
            dname=itd
            audiopath=audio+'/'+dname
            labpath=lab+'/'+dname
            rootpath=root+'/'+dname
            datapath=data+'/'+dname
            if os.path.exists(datapath)==False:
                os.makedirs(datapath)
            else:
                print 'SKIP',dname,datapath,'exist'
                continue
            confPtr=open(os.path.join(datapath,'myconf.txt'),'w')
            confPtr.write(os.path.join(wav10dir,dname))
            confPtr.close()
            if os.path.exists(os.path.join(wav10dir,dname))==False:
                os.makedirs(os.path.join(wav10dir,dname))
            else:
                print 'SKIP',os.path.join(wav10dir,dname),'exist'
                continue
            print 'START',dname
            cmdstr="%s %d %s %s %s %s %d"%(exe,tasktype,rootpath,datapath,audiopath,labpath,dmp3)
            ret=os.system(cmdstr);

if os.path.exists(root)==False:
    os.makedirs(root)

lis=os.listdir(lab)
t_num=5
t_array=[]
llen=len(lis)
per=int(llen/t_num)
for i in range(t_num):
    t_array.append(run_hmm())
for i in range(t_num):
    if i==t_num-1:
        sub_lis=lis[i*per:]
    else:
        sub_lis=lis[i*per:(i+1)*per]
    t_array[i].set_param(sub_lis)
for i in range(t_num):
    t_array[i].start()
