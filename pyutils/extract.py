#!/usr/local/bin/python
import sys
import os
sys.path.append('/home/zwang/project/bnsys')
from genData import *
def validY_resi(l,R,th):
    for r in R:
        if len(r[2])<5 or r[2][:5]!='cough':
            continue
        elif l[0]-th>r[1] or l[1]+th<r[0]:
            continue
        else:
            return 1
    return 0
def extract_resi(wdir,wavpath,recpath,labpath):
    utter_list=[]
    scale=1e-7
    prefix=os.path.splitext(os.path.split(wavpath)[1])[0]
    R=loadLab(recpath,scale);
    L=loadLab(labpath,scale);
    wr=wave.open(wavpath,'r')
    srate=wr.getframerate()
    for l in L:
        flag=l[2]
        if len(flag)>=5 and flag[:5]=='cough':
            v0=validY_resi(l,R,0);
            v1=validY_resi(l,R,1.0);
            if v0 or v1:
                continue
            oname=str(v0)+'_'+str(v1)+'_'+prefix+'_'+str(len(utter_list))+'_'+flag+'.wav'
            opath=os.path.join(wdir,oname)
            utter_list.append(opath)
            ww=wave.open(opath,'w')
            st=l[0]
            et=l[1]
            sf=int(st*srate)
            ef=int(math.ceil(et*srate))
            ww.setnchannels(wr.getnchannels())
            ww.setsampwidth(wr.getsampwidth())
            ww.setframerate(wr.getframerate())
            wr.setpos(sf)
            buf=wr.readframes(ef-sf)
            ww.setnframes(ef-sf)
            ww.writeframes(buf)
            ww.close()
    wr.close()
    return utter_list
featuredir='/home/zwang/data/hmm_mfcc'
outDir='/home/zwang/data/exp_cv'
wavDir='/home/zwang/data/wav10s'
wdir='/home/zwang/s1res'

nfold=2
lis=os.listdir(outDir)
for itd in lis:
    if os.path.isdir(outDir+'/'+itd)==False:
        continue
    if os.path.exists(outDir+'/'+itd+'/timeLog.txt')==False:
        print 'skip',itd
        continue
    dname=itd
    featurepath=featuredir+'/'+dname
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
            wavpath=wavDir+'/'+dname+'/'+itf+'.wav'
            recpath=predictLabPath+'/'+itf+'.lab'
            labpath=trueLabPath+'/'+itf+'.lab'
            extract_resi(wdir,wavpath,recpath,labpath)
