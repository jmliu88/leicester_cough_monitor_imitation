#!/usr/local/bin/python
import os
import sys
import pdb

def foo(logname,predicted):
    outpath='evaluate/'+logname+'.log'
    test='evaluate/target.list'
    #expected='/home/zwang/data/hmm_mfcc'
    expected='/home/james/hmm/4h/splitedLabs'
    thresh=-1e7#1e7
    tp=0
    pp=0
    p=0
    hit=0
    tenCount=0
    acceptCount=0
    rejectCount=0
    with open('data/sipid.list','r') as fid:
        sipid = [t.rstrip() for t in fid.readlines()]
    wptr=open(outpath,'w')
    with open(test,'r') as T:
        for t in T:
            t=t.strip()
            tenCount+=1
            dname=t.split('_')[0]
            if dname in sipid:
                continue
            predictLabPath=os.path.join(predicted,t+'.lab')
            #trueLabPath=os.path.join(expected,dname,'splitedLabs',t+'.lab');
            trueLabPath=os.path.join(expected,t+'.lab');
            trueLab=[]
            predictLab=[]
            if os.path.exists(predictLabPath):
                fptr=open(predictLabPath,'r')
                while True:
                    rec=fptr.readline()
                    rec=rec.strip()
                    if len(rec)==0:
                        break
                    ret=rec.split(' ')
                    if len(ret)>=3 and ret[2][0:5]=='cough':
                        st=float(ret[0])
                        et=float(ret[1])
                        if predictLab==[] or st-predictLab[-1][1]>thresh:
                            predictLab.append([st,et])
                        else:
                            predictLab[-1][1]=et
                fptr.close()
            #get true lab
            fptr=open(trueLabPath)
            #if trueLabPath.find('47M') != -1:
            #    pdb.set_trace()
            while True:
                rec=fptr.readline()
                rec=rec.strip()
                #pdb.set_trace()
                if len(rec)==0:
                    break
                ret=rec.split('\t')
                if len(ret)>=3 and ret[2][0:5]=='cough':
                    st=float(ret[0])
                    et=float(ret[1])
                    if trueLab==[] or st-trueLab[-1][1]>thresh:
                        trueLab.append([st,et])
                    else:
                        trueLab[-1][1]=et
            fptr.close()

            chash=[0 for o in range(len(trueLab))]
            p+=len(trueLab)
            pp+=len(predictLab)
            for sa in predictLab:
                ok=False
                for i in range(len(trueLab)):
                    sb=trueLab[i]

                    #if not (sa[0]+0.016>sb[1] or sa[1]-0.016<sb[0]): # two intervals intersect
                    if sb[0]<(sa[0]+sa[1])/2<sb[1]: # middle of true lays in pre
                          ok=True
                          chash[i]=1
                if ok:
                    hit+=1
            tp+=sum(chash)

    duration=float(tenCount*10.0/3600)
    precision=float(tp)/pp
    recall=float(tp)/p
    f1=2*precision*recall/(precision+recall)
    fa=float(pp-tp)/duration
    faall=float(pp-tp)

    wptr.write('duration:'+str(duration)+'\n')
    wptr.write('N of total cough:'+str(p)+'\n')
    wptr.write('N of total detected:'+str(pp)+'\n')
    wptr.write('N of false alarms:'+ str(faall)+'\n')
    wptr.write('tp:'+str(tp)+'\n')
    wptr.write('hit:'+str(hit)+'\n')
    wptr.write('precision:'+str(precision)+'\n')
    wptr.write('recall:'+str(recall)+'\n')
    wptr.write('f1:'+str(f1)+'\n')
    wptr.write('fa:'+str(fa)+'\n')
    wptr.write('accept segment'+str(acceptCount)+'\n')
    wptr.write('reject segment'+str(rejectCount)+'\n')
    wptr.close()

if __name__=='__main__':
    if len(sys.argv)<=3:
        print '3 param: s2fTestList.txt, savename, prediction directory'
        exit(0)
    os.system('local/prepEvaluate.sh %s'%sys.argv[1])
    foo(*sys.argv[2:])
