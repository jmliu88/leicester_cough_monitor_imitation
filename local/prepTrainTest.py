#!/usr/local/bin/python
import os
import sys
data='/home/zwang/data/leaveout'
exp='/home/zwang/project/hmm/exp/small'
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
    pid=loadList('data/pid.list')
    cli=loadList(data+'/labeledlist.txt')
    fli=loadList(data+'/emptylist.txt')
    f2c=10
    trainCough=cli[:100]
    trainFiller=fli[:100*f2c]
    test=cli[100:200]+fli[100*f2c:100*f2c+100]
    saveList(exp+'/s1cTrainList.txt',trainCough)
    saveList(exp+'/s1fTrainList.txt',trainFiller)
    saveList(exp+'/s2TestList.txt',test)
if __name__=='__main__':
    if os.path.exists(exp)==False:
        os.makedirs(exp)
    foo()
