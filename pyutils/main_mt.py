import sys
import os
import commands
from shutil import rmtree
import threading

nfold=1
datadir='/home/zwang/data/wav'
labdir='/home/zwang/data/TextGrids'
featuredir='/home/zwang/data/hmm_mfcc'
rootdir='/home/zwang/data/exp/'
runPath='/home/zwang/hmmcough'
task_type=2
filler_exp=7

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
            #print subPath
            dname=itd
            if dname not in ['ZZL']:
                continue
            datapath=datadir+'/'+dname
            labpath=labdir+'/'+dname
            rootpath=rootdir+'/'+dname
            featurepath=featuredir+'/'+dname
            print dname
            cmdstr="%s %s %s %s %s %s %d %d %d"%(runPath,dname,datapath,labpath,rootpath,featurepath,nfold,task_type,filler_exp)
            ret=os.system(cmdstr);

if os.path.exists(featuredir)==False:
    os.makedirs(featuredir)
if os.path.exists(rootdir)==False:
    os.makedirs(rootdir)

lis=os.listdir(labdir)
t_num=3
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

'''
    if ret==0:
        for idn in range(0,nfold):
            rmtree(rootpath+"/"+str(idn)+"/s1cLabs");
            rmtree(rootpath+"/"+str(idn)+"/s1fLabs");
            rmtree(rootpath+"/"+str(idn)+"/elabs");
'''
                
