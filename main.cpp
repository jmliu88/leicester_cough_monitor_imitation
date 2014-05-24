#include "stdio.h"
#include "time.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <cstring>
#include <string>
#include "src/glb.h"
#include "src/Util.h"
#include "src/PreProcess.h"
#include "src/CoughTrain.h"
#include "src/FillerTrain.h"
#include "src/EmbeddedTrain.h"
#include "src/Recognition.h"
#include "src/CommonTrain.h"
#include "src/htks.h"
#include "trainModel.h"
#include "detectS2.h"
#include "src/mfccReader.h"

struct segment_
{
	double u;
	double v;
	segment_(double uu,double vv)
	{
		u=uu;
		v=vv;
	}
};
double costFeature,costTrain,costTest;
struct idx_v
{
	int idx;
	int rval;
};

void doPreProcess(std::string outDir,std::string srcAudio,std::string srcLab,bool dmp3)
{
	bool isTG=true;
	int segSec=10;
	PreProcess pp;
	createEmptyDir(outDir);
	printf("preprocess start...\n");
	pp.init(outDir,srcAudio,srcLab);
	pp.run(segSec,segSec,isTG,dmp3);
}

bool kcmp(struct idx_v a, struct idx_v b)
{
	return a.rval<b.rval;
}

void cvIndex(int* eidx,int elen,int nfold)
{
	int i;
	int j;
	struct idx_v *pr=new struct idx_v[elen];

	for(i=0;i<elen;i++)
	{
		eidx[i]=-1;
		pr[i].idx=i;
		pr[i].rval=rand();
	}

	std::sort(pr,pr+elen,kcmp);
	int evtPerFold=elen/nfold;
	for(i=0;i<nfold;i++)
	{
		for(j=0;j<evtPerFold;j++)
		{
			eidx[pr[i*evtPerFold+j].idx]=i;
		}
	}
	for(j=nfold*evtPerFold;j<elen;j++)
	{
		eidx[pr[j].idx]=nfold-1;
	}
	delete pr;
}

void train_model(std::string srcDir,std::string workingDir,int nfold,int fillerExp)
{
	int i,j;
	createEmptyDir(workingDir);
	std::string labeledListPath=srcDir+"/labeledlist.txt";
	std::string splitedLabelDir=srcDir+"/splitedLabs";
	std::string emptyListPath=srcDir+"/emptylist.txt";
	std::vector<std::string> labeledMfccList=readListFile(labeledListPath);
	std::vector<std::string> emptyMfccList=readListFile(emptyListPath);

	int coughIt=1;//4;
	int ebItP=1;//3;
	double var=0.01;
	int f2c=10;
	int minFseg=1200;

	int clen=labeledMfccList.size();
	int *cidx=new int[clen];
	cvIndex(cidx,clen,nfold);

	int flen=emptyMfccList.size();
	int *fidx=new int[flen];
	cvIndex(fidx,flen,nfold);

	/*cross validation*/
	for(i=0;i<nfold;i++)
	{
		std::string cvSubPath;
		std::string s1cTrainListPath;
		std::string s1fTrainListPath;
		std::string s1TestListPath;

		ss.clear();
		ss<<i;
		ss>>cvSubPath;
		ss.clear();
		cvSubPath=workingDir+"/"+cvSubPath;
		createEmptyDir(cvSubPath);

		s1cTrainListPath=cvSubPath+"/"+"s1cTrainList.txt";
		s1fTrainListPath=cvSubPath+"/"+"s1fTrainList.txt";
		s1TestListPath=cvSubPath+"/"+"s2TestList.txt";

		std::vector<std::string> cVec;
		std::vector<std::string> fVec;
		std::vector<std::string> tVec;
		int ccount=0;
		for(j=0;j<clen;j++)
			if(cidx[j]!=i)
				ccount++;
		for(j=0;j<clen;j++)
		{
			if(cidx[j]!=i)
				cVec.push_back(labeledMfccList[j]);
			else
				tVec.push_back(labeledMfccList[j]);
		}

		int fcount=0;
		for(j=0;j<flen;j++)
			if(fidx[j]!=i)
				fcount++;
		if(fcount>ccount*f2c)
			fcount=ccount*f2c;
		if(fcount<minFseg)
			fcount=minFseg;
		for(j=0;j<flen;j++)
		{
			if(fidx[j]!=i)
			{
				if(fVec.size()<fcount)
					fVec.push_back(emptyMfccList[j]);
			}
			else
				tVec.push_back(emptyMfccList[j]);
		}
		writeListFile(s1cTrainListPath,cVec);
		writeListFile(s1fTrainListPath,fVec);
		writeListFile(s1TestListPath,tVec);

		time_t s1,s2,s3;
time(&s1);

		printf("fold-%d stage-1 cough train\n",i);
		CoughTrain ct;
		ct.init(cvSubPath,s1cTrainListPath,splitedLabelDir);
		ct.run(coughIt);

		printf("fold-%d stage-1 filler train\n",i);
		FillerTrain ft;
		ft.init(cvSubPath,s1fTrainListPath);
		ft.run(fillerExp,var);

		printf("fold-%d stage-1 embedded train\n",i);
		EmbeddedTrain et;
		et.init(cvSubPath,ct.getScoredLabelDir(),ft.getFillerLabelDir(),ct.getOutputDir(),ft.getOutputDir(),s1cTrainListPath,s1fTrainListPath);
		et.run(ebItP);
time(&s2);
		printf("fold-%d stage-1 detect\n",i);
		Recognition rc;
		rc.init(cvSubPath,et.getCMMF(),et.getFMMF(),et.getHmmList());
		rc.run(s1TestListPath);
time(&s3);
		costTrain+=difftime(s2,s1);
		costTest+=difftime(s3,s2);
	}
	delete cidx;
	delete fidx;
}
void train(std::string srcDir,std::string workingDir,int fillerExp)
{
	int i=0,j=0;
	createEmptyDir(workingDir);
	std::string labeledListPath=srcDir+"/labeledlist.txt";
	std::string splitedLabelDir=srcDir+"/splitedLabs";
	std::string emptyListPath=srcDir+"/emptylist.txt";
	std::vector<std::string> labeledMfccList=readListFile(labeledListPath);
	std::vector<std::string> emptyMfccList=readListFile(emptyListPath);

	int coughIt=1;//4;
	int ebItP=1;//3;
	double var=0.01;
	int f2c=10;
	int minFseg=1200;

	int clen=labeledMfccList.size();

	int flen=emptyMfccList.size();

    std::string cvSubPath;
    std::string s1cTrainListPath;
    std::string s1fTrainListPath;
    std::string s1TestListPath;

    ss.clear();
    ss<<i;
    ss>>cvSubPath;
    ss.clear();
    cvSubPath=workingDir+"/"+cvSubPath;
    createEmptyDir(cvSubPath);

    s1cTrainListPath=cvSubPath+"/"+"s1cTrainList.txt";
    s1fTrainListPath=cvSubPath+"/"+"s1fTrainList.txt";
    s1TestListPath=cvSubPath+"/"+"s2TestList.txt";

    std::vector<std::string> cVec;
    std::vector<std::string> fVec;
    std::vector<std::string> tVec;
    int ccount=0;
    for(j=0;j<clen;j++)
        ccount++;
    for(j=0;j<clen;j++)
    {
        cVec.push_back(labeledMfccList[j]);
        tVec.push_back(labeledMfccList[j]);
    }

    int fcount=0;
    for(j=0;j<flen;j++)
        fcount++;
    if(fcount>ccount*f2c)
        fcount=ccount*f2c;
    if(fcount<minFseg)
        fcount=minFseg;
    for(j=0;j<flen;j++)
    {
        if(fVec.size()<fcount)
            fVec.push_back(emptyMfccList[j]);
        tVec.push_back(emptyMfccList[j]);
    }
    writeListFile(s1cTrainListPath,cVec);
    writeListFile(s1fTrainListPath,fVec);
    writeListFile(s1TestListPath,tVec);

    time_t s1,s2,s3;
time(&s1);

    printf("fold-%d stage-1 cough train\n",i);
    CoughTrain ct;
    ct.init(cvSubPath,s1cTrainListPath,splitedLabelDir);
    ct.run(coughIt);

    printf("fold-%d stage-1 filler train\n",i);
    FillerTrain ft;
    ft.init(cvSubPath,s1fTrainListPath);
    ft.run(fillerExp,var);

    printf("fold-%d stage-1 embedded train\n",i);
    EmbeddedTrain et;
    et.init(cvSubPath,ct.getScoredLabelDir(),ft.getFillerLabelDir(),ct.getOutputDir(),ft.getOutputDir(),s1cTrainListPath,s1fTrainListPath);
    et.run(ebItP);
time(&s2);
    printf("fold-%d stage-1 detect\n",i);
    Recognition rc;
    rc.init(cvSubPath,et.getCMMF(),et.getFMMF(),et.getHmmList());
    rc.run(s1TestListPath);
time(&s3);
    costTrain+=difftime(s2,s1);
    costTest+=difftime(s3,s2);
}
int main(int argc, char* argv[])
{
	std::string prefix(argv[1]);
	std::string srcAudio=std::string(argv[2]);
	std::string srcLab=std::string(argv[3]);
	std::string rootDir=std::string(argv[4]);
	std::string dataDir=std::string(argv[5]);
	int nfold=atoi(argv[6]);
	int task_type=atoi(argv[7]);
	int filler_exp=atoi(argv[8]);
	bool dmp3=atoi(argv[9]);
	costFeature=0;
	costTrain=0;
	costTest=0;
	time_t s1,s2,s3;
	time(&s1);
	if ((task_type&1)!=0)
		doPreProcess(dataDir,srcAudio,srcLab,dmp3);
	time(&s2);
	if ((task_type&2)!=0)
		train_model(dataDir,rootDir,2,filler_exp);
		//train(dataDir,rootDir,filler_exp);
	//test_model(dataDir,rootDir);
	time(&s3);

	costFeature=difftime(s2,s1);
	char timeStr[64];
	std::string timeLogPath=rootDir+"/timeLog.txt";
	FILE* timeLogFptr=fopen(timeLogPath.c_str(),"w");
	fprintf(timeLogFptr,"costFeature:%lf\n",costFeature/60.0);
	fprintf(timeLogFptr,"costTrain:%lf\n",costTrain/60.0);
	fprintf(timeLogFptr,"costTest:%lf\n",costTest/60.0);

	strftime(timeStr,sizeof(timeStr),"%Y/%m/%d %X\n",localtime(&s1));
	fputs(timeStr,timeLogFptr);
	strftime(timeStr,sizeof(timeStr),"%Y/%m/%d %X\n",localtime(&s2));
	fputs(timeStr,timeLogFptr);
	strftime(timeStr,sizeof(timeStr),"%Y/%m/%d %X\n",localtime(&s3));
	fputs(timeStr,timeLogFptr);

	fclose(timeLogFptr);
	return 0;
}
