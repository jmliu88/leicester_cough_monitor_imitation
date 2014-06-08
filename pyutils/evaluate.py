#!/usr/local/bin/python
import sys
import os
#sys.path.append('/home/zwang/workspace/htkIO')
#from htkIO import isAccept

featuredir='/home/zwang/data/hmm_mfcc'
outDir='/home/zwang/data/exp_cv'

factor=1.0
thresh=1e7#ns
nfold=2
outname='evaluate_'+str(int(thresh/1e7))+'s_e.txt'

acceptCount=0
rejectCount=0

wptr=open(outDir+'/'+outname,'w')
lis=os.listdir(outDir)
for itd in lis:
    if os.path.isdir(outDir+'/'+itd)==False:
        continue
    if os.path.exists(outDir+'/'+itd+'/timeLog.txt')==False:
        print 'skip',itd
        continue
    tp=0
    pp=0
    p=0
    hit=0
    tenCount=0
    dname=itd
    featurepath=featuredir+'/'+dname

    #lis=os.listdir(featurepath)
    #for itf in lis:
    #    dummy,ext=os.path.splitext(itf)
    #    if ext=='.inf':
    #        tenCount=tenCount+1
    cut_inf_ptr=open(featurepath+'/cut.inf')
    tenCount=len(cut_inf_ptr.readlines())
    cut_inf_ptr.close()
    tenCount = tenCount*factor
    print '#'+dname,str(tenCount/6.0)

    for idn in range(0,nfold):

        predictLabPath=outDir+'/'+dname+'/'+str(idn)+'/'+'s1rec'
        trueLabPath=featuredir+'/'+dname+'/'+'splitedLabs';

        fptr=open(outDir+'/'+dname+'/'+str(idn)+'/s2TestList.txt','r')
        lf=[]
        while True:
            li=fptr.readline()
            if len(li)==0:
                break
            lf.append(li)
        fptr.close()

        for itf in lf:
            dummy,itf=os.path.split(itf)
            itf,dummy=os.path.splitext(itf)
            itf=itf+".lab"
            trueLab=[]
            predictLab=[]
            fptr=open(predictLabPath+'/'+itf)
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
            fptr=open(trueLabPath+'/'+itf)
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

    duration=float(tenCount*10.0/60)
    precision=float(tp)/pp
    recall=float(tp)/p
    f1=2*precision*recall/(precision+recall)
    fa=float(pp-tp)/duration

    wptr.write('#'+dname+'\n')
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




