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
#include "src/mfccReader.h"

double cost,costTrain,costTest;

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


void train(std::string srcDir,std::string workingDir,int fillerExp)
{
	int coughIt=1;//4;
	int ebItP=1;//3;
	double var=0.01;
    std::string s1cTrainListPath=workingDir+"/"+"s1cTrainList.txt";
    std::string s1fTrainListPath=workingDir+"/"+"s1fTrainList.txt";
    std::string s1TestListPath=workingDir+"/"+"s2TestList.txt";
    std::string splitedLabelDir=srcDir+"/splitedLabs";
    createEmptyDir(workingDir);
    time_t s1,s2,s3;

    time(&s1);
    printf("stage-1 cough train\n");
    CoughTrain ct;
    ct.init(workingDir,s1cTrainListPath,splitedLabelDir);
    ct.run(coughIt);

    printf("stage-1 filler train\n");
    FillerTrain ft;
    ft.init(workingDir,s1fTrainListPath);
    ft.run(fillerExp,var);

    printf("stage-1 embedded train\n");
    EmbeddedTrain et;
    et.init(workingDir,ct.getScoredLabelDir(),ft.getFillerLabelDir(),ct.getOutputDir(),ft.getOutputDir(),s1cTrainListPath,s1fTrainListPath);
    et.run(ebItP);

    time(&s2);
    printf("stage-1 detect\n");
    Recognition rc;
    rc.init(workingDir,et.getCMMF(),et.getFMMF(),et.getHmmList());
    rc.run(s1TestListPath);

    time(&s3);
    costTrain+=difftime(s2,s1);
    costTest+=difftime(s3,s2);
}
void test(std::string modelDir,std::string testPath,std::string workingDir)
{
    std::string cmmf=modelDir+"/eMMf.mmf";
    std::string fmmf=modelDir+"/eMMf2.mmf";
    std::string hmmlist=modelDir+"/eHmmlist.txt";
    createEmptyDir(workingDir);

    printf("stage-1 detect\n");
    Recognition rc;
    rc.init(workingDir,cmmf,fmmf,hmmlist);
    rc.run(testPath);
}
int main(int argc, char* argv[])
{
	cost=0;
	costTrain=0;
	costTest=0;
	time_t s1,s2,s3;
	int task_type=atoi(argv[1]);
	if(task_type==0)
	{
		std::string rootDir=std::string(argv[2]);
		std::string dataDir=std::string(argv[3]);
		std::string srcAudio=std::string(argv[4]);
		std::string srcLab=std::string(argv[5]);
		bool dmp3=atoi(argv[6]);
		time(&s1);
		doPreProcess(dataDir,srcAudio,srcLab,dmp3);
		time(&s2);
		cost=difftime(s2,s1);
		char timeStr[64];
		std::string timeLogPath=rootDir+"/preprocess.log";
		FILE* timeLogFptr=fopen(timeLogPath.c_str(),"w");
		fprintf(timeLogFptr,"cost:%lf\n",cost/60.0);
		strftime(timeStr,sizeof(timeStr),"%Y/%m/%d %X\n",localtime(&s1));
		fputs(timeStr,timeLogFptr);
		strftime(timeStr,sizeof(timeStr),"%Y/%m/%d %X\n",localtime(&s2));
		fputs(timeStr,timeLogFptr);
		strftime(timeStr,sizeof(timeStr),"%Y/%m/%d %X\n",localtime(&s3));
		fputs(timeStr,timeLogFptr);
		fclose(timeLogFptr);
	}
	else if(task_type==1)
	{
		std::string rootDir=std::string(argv[2]);
		std::string dataDir=std::string(argv[3]);
		int filler_exp=atoi(argv[4]);
		time(&s1);
		train(dataDir,rootDir,filler_exp);
		time(&s2);
		cost=difftime(s2,s1);
		char timeStr[64];
		std::string timeLogPath=rootDir+"/train.log";
		FILE* timeLogFptr=fopen(timeLogPath.c_str(),"w");
		fprintf(timeLogFptr,"cost:%lf\n",cost/60.0);
		fprintf(timeLogFptr,"costTrain:%lf\n",costTrain/60.0);
		fprintf(timeLogFptr,"costTest:%lf\n",costTest/60.0);
		strftime(timeStr,sizeof(timeStr),"%Y/%m/%d %X\n",localtime(&s1));
		fputs(timeStr,timeLogFptr);
		strftime(timeStr,sizeof(timeStr),"%Y/%m/%d %X\n",localtime(&s2));
		fputs(timeStr,timeLogFptr);
		strftime(timeStr,sizeof(timeStr),"%Y/%m/%d %X\n",localtime(&s3));
		fputs(timeStr,timeLogFptr);
		fclose(timeLogFptr);
	}
    else if(task_type==2)
    {
        std::string rootDir=std::string(argv[2]);
        std::string testPath=std::string(argv[3]);
        std::string modelDir=std::string(argv[4]);
		time(&s1);
		test(modelDir,testPath,rootDir);
		time(&s2);
		cost=difftime(s2,s1);
		char timeStr[64];
		std::string timeLogPath=rootDir+"/test.log";
		FILE* timeLogFptr=fopen(timeLogPath.c_str(),"w");
		fprintf(timeLogFptr,"cost:%lf\n",cost/60.0);
		strftime(timeStr,sizeof(timeStr),"%Y/%m/%d %X\n",localtime(&s1));
		fputs(timeStr,timeLogFptr);
		strftime(timeStr,sizeof(timeStr),"%Y/%m/%d %X\n",localtime(&s2));
		fputs(timeStr,timeLogFptr);
		strftime(timeStr,sizeof(timeStr),"%Y/%m/%d %X\n",localtime(&s3));
		fputs(timeStr,timeLogFptr);
		fclose(timeLogFptr);
    }
	else
	{
		printf("unknown tasktype\n");
	}

	return 0;
}
