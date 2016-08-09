#!/usr/local/bin/python
import os
import sys
def foo(logname,predicted):
    outpath='evaluate/'+logname+'.log'
    test='evaluate/target.list'
    expected='/home/zwang/data/hmm_mfcc'
    thresh=-1e7#1e7
    tp=0
    pp=0
    p=0
    hit=0
    tenCount=0
    acceptCount=0
    rejectCount=0

    personId={}
    groundPositive={}
    predictedPositive={}
    truePositive={}
    hitPositive={}
    segCount={}

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
            pPID=len(trueLab)
            ppPID=len(predictLab)
            hitPID=0
            tpPID=0
            if personId.has_key(dname)==False:
                personId[dname]=True
                groundPositive[dname]=pPID
                truePositive[dname]=0
                hitPositive[dname]=0
                segCount[dname]=1
                predictedPositive[dname]=ppPID
            else:
                groundPositive[dname]=groundPositive[dname]+pPID
                predictedPositive[dname]=predictedPositive[dname]+ppPID
                segCount[dname]=segCount[dname]+1
            for sa in predictLab:
                ok=False
                for i in range(len(trueLab)):
                    sb=trueLab[i]
                    if not (sa[0]+0.016>sb[1] or sa[1]-0.016<sb[0]):
                          ok=True
                          chash[i]=1
                if ok:
                    hitPID+=1
                    hit+=1
            tp+=sum(chash)
            tpPID+=sum(chash)
            truePositive[dname]+=tpPID
            hitPositive[dname]+=hitPID

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
    #wptr.write('accept segment'+str(acceptCount)+'\n')
    #wptr.write('reject segment'+str(rejectCount)+'\n')
    wptr.close()

    for it in personId.iteritems():
        pid=it[0]
        fa=predictedPositive[pid]-hitPositive[pid]
        print '#%s'%pid
        print 'ground positive: %d'%groundPositive[pid]
        print 'predicted positive: %d'%predictedPositive[pid]
        print 'hit positive: %d'%hitPositive[pid]
        print 'recall positive: %d'%truePositive[pid]
        print 'recall: %.3f'%(float(truePositive[pid])/groundPositive[pid])
        print 'false alarm: %d'%(fa)
        print 'duration(minute): %d'%(segCount[pid]/6)
        print 'false alarm per hour: %.1f'%(fa/(segCount[pid]/360.0))

if __name__=='__main__':
    if len(sys.argv)<=3:
        print '3 param'
        exit(0)
    os.system('local/prepEvaluate.sh %s'%sys.argv[1])
    foo(*sys.argv[2:])
