#!/usr/local/bin/python
import sys
import os
import commands
from shutil import rmtree
import threading

nfold=2
dmp3=1
datadir='/home/zwang/data/audio'
labdir='/home/zwang/data/TextGrids'
featuredir='/home/zwang/data/hmm_mfcc'
rootdir='/home/zwang/data/exp_cv/'
runPath='/home/zwang/project/hmm/Debug/hmm'
task_type=3
filler_exp=6
wav10dir='/home/zwang/data/wav10s'
class run_hmm(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
    def set_param(self,lis):
        self.lis=lis
    def run(self):
        lis=self.lis
        for itd in lis:
            subPath=datadir+'/'+itd
            if os.path.isdir(subPath)==False:
                continue;
            dname=itd
            datapath=datadir+'/'+dname
            labpath=labdir+'/'+dname
            rootpath=rootdir+'/'+dname
            featurepath=featuredir+'/'+dname
            if task_type&1 == 1:
                if os.path.exists(featurepath)==False:
                    os.makedirs(featurepath)
                else:
                    print 'SKIP',dname,featurePath,'exist'
                    continue
                confPtr=open(os.path.join(featurepath,'myconf.txt'),'w')
                confPtr.write(os.path.join(wav10dir,dname))
                confPtr.close()
                if os.path.exists(os.path.join(wav10dir,dname))==False:
                    os.makedirs(os.path.join(wav10dir,dname))
                else:
                    print 'SKIP',os.path.join(wav10dir,dname),'exist'
                    continue
            print 'START',dname
            cmdstr="%s %s %s %s %s %s %d %d %d %d"%(runPath,dname,datapath,labpath,rootpath,featurepath,nfold,task_type,filler_exp,dmp3)
            ret=os.system(cmdstr);

if os.path.exists(featuredir)==False:
    os.makedirs(featuredir)
if os.path.exists(rootdir)==False:
    os.makedirs(rootdir)

lis=os.listdir(labdir)
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
