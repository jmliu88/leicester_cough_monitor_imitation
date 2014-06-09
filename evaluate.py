#!/usr/local/bin/python
import os
import sys
stp='s1'
outpath='evaluate/'+stp+'.txt'
test='evaluate/test.list'
expected='/home/zwang/data/hmm_mfcc'
predicted='exp/big/s1rec'
thresh=1e7
def foo():
    tp=0
    pp=0
    p=0
    hit=0
    tenCount=0
    acceptCount=0
    rejectCount=0
    wptr=open(outpath,'w')
    with open(test,'r') as T:
        for t in T:
            t=t.strip()
            tenCount+=1
            dname=t.split('_')[0]
            predictLabPath=os.path.join(predicted,t+'.lab')
            trueLabPath=os.path.join(expected,dname,'splitedLabs',t+'.lab');
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
            while True:
                rec=fptr.readline()
                rec=rec.strip()
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
                    if not (sa[0]>=sb[1] or sa[1]<=sb[0]):
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

    wptr.write('duration:'+str(duration)+'\n')
    wptr.write('p:'+str(p)+'\n')
    wptr.write('pp:'+str(pp)+'\n')
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
    foo()
