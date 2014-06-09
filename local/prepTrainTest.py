#!/usr/local/bin/python
import random
import numpy as np
import os
import sys
data='/home/zwang/data/leaveout'
exp='/home/zwang/project/hmm/exp/big'
def loadList(lpath):
    li=[]
    with open(lpath,'r') as P:
        for p in P:
            li.append(p.strip())
    return li
def saveList(lpath,li):
    fptr=open(lpath,'w')
    for l in li:
        fptr.write(l+'\n')
    fptr.close()
def foo():
    os.system('. local/preparePID.sh')
    pid=np.array(loadList('data/pid.list'))
    cli=np.array(loadList(data+'/labeledlist.txt'))
    fli=np.array(loadList(data+'/emptylist.txt'))
    f2c=2
    coughTotal=cli.shape[0]
    fillerTotal=fli.shape[0]
    coughtrain=int(coughTotal*2.0/3)
    fillertrain=min(coughtrain*f2c,fillerTotal)
    cidx=np.zeros((coughTotal),dtype=bool)
    randx=random.sample(xrange(coughTotal),coughtrain)
    cidx[randx]=True
    fidx=np.zeros((fillerTotal),dtype=bool)
    randx=random.sample(xrange(fillerTotal),fillertrain)
    fidx[randx]=True

    divc=int(coughtrain*2.0/3)
    divf=int(fillertrain*2.0/3)
    trainCough=cli[cidx][:divc]
    trainFiller=fli[fidx][:divf]
    devCough=cli[cidx][divc:]
    devFiller=fli[fidx][divf:]

    test=np.hstack((cli[~cidx],fli[~fidx]))
    saveList(exp+'/s1cTrainList.txt',cli[cidx])
    saveList(exp+'/s1fTrainList.txt',fli[fidx])
    saveList(exp+'/s2cTrainList.txt',trainCough)
    saveList(exp+'/s2fTrainList.txt',trainFiller)
    saveList(exp+'/s2cDevList.txt',devCough)
    saveList(exp+'/s2fDevList.txt',devFiller)
    saveList(exp+'/s2TestList.txt',test)
if __name__=='__main__':
    if os.path.exists(exp)==False:
        os.makedirs(exp)
    foo()
