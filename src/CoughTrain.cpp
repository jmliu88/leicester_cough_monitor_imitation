#pragma once
#include "glb.h"
#include "CoughTrain.h"
#include "Util.h"
#include "htks.h"
#include "stdio.h"
#include <set>

std::string CoughTrain::getOutputDir()
{
	return outputDir;
}
std::string CoughTrain::getScoredLabelDir()
{
	return scoredLabelDir;
}
std::string CoughTrain::getMfccList()
{
	return mfccList;
}
void CoughTrain::init(std::string inWorkingDir,std::string inMfccList,std::string inLabelDir)
{
	workingDir=inWorkingDir;
	mfccList=inMfccList;
	labelDir=inLabelDir;
	tmpDir=workingDir+"/"+"tmp";
	modelDir=workingDir+"/"+"s1model";
	scoredLabelDir=workingDir+"/"+"s1cLabs";
	outputDir=modelDir+"/"+"cough";
}
void CoughTrain::run(int repTimes)
{
	createEmptyDir(workingDir);
	createEmptyDir(tmpDir);
	createEmptyDir(modelDir);
	createEmptyDir(scoredLabelDir);
	createEmptyDir(outputDir);

	unsigned int i,j,iterCount=0;
	int nsl=sizeof(ns)/sizeof(int);
	std::vector<std::string> vec;

	for(i=0;i<nsl;i++)
	{
		std::string mmfname;
		ss<<"cough"<<ns[i];
		ss>>mmfname;
		ss.clear();
		modellist.push_back(mmfname);
	}

	std::string itemp;
	std::string curdir,predir;
	//make conf and proto
	for(i=0;i<modellist.size();i++)
		writeCoughProtoFile(modelDir+"/"+modellist[i]+"."+hmmExt,gTarKind,ns[i],39,1);
	//init hmm_flat
	curdir=modelDir+"/"+"hflat";
	createEmptyDir(curdir);
	WrapHCompV wc;
	std::string hmmName=modelDir+"/"+(modellist[0]+"."+hmmExt);
	wc.init(mfccList,curdir,hmmName,modellist[0]);
	wc.run();

	//init hmm
	ss<<iterCount++;
	ss>>itemp;
	ss.clear();
	curdir=modelDir+"/"+"h"+itemp;
	createEmptyDir(curdir);
	WrapHInit wi;
	WrapHRest wr;
	for(i=0;i<modellist.size();i++)
	{
		hmmName=modelDir+"/"+(modellist[i]+"."+hmmExt);
		wi.init(mfccList,curdir,hmmName,"cough",labelDir,modellist[i]);
		//wi.init(sublabList[i],curdir,hmmName,"cough",labelDir,modellist[i]);
		wi.run();

		//hmmName=curdir+"/"+(modellist[i]+"."+hmmExt);
		//wr.init(sublabList[i],curdir,hmmName,"cough",labelDir,modellist[i],"");
		//wr.run();
	}

	//res hmm
	for(int it=0;it<repTimes;it++)
	{
		//train cough
		predir=curdir;
		ss<<iterCount++;
		ss>>itemp;
		ss.clear();
		curdir=modelDir+"/"+"h"+itemp;
		if(it+1==repTimes)
			curdir=outputDir;
		else
			createEmptyDir(curdir);
		for(i=0;i<modellist.size();i++)
		{
			hmmName=predir+"/"+(modellist[i]+"."+hmmExt);
			std::string scorePath=tmpDir+"/"+modellist[i]+".score";
			wr.init(mfccList,curdir,hmmName,"cough",labelDir,modellist[i],scorePath);
			wr.run();
		}
		
		//update label
		updateLabels(scoredLabelDir,labelDir,tmpDir,mfccList);
		for(i=0;i<modellist.size();i++)
		{
			hmmName=curdir+"/"+(modellist[i]+".hmm");
			//wi.init(mfccList,curdir,hmmName,modellist[i],scoredLabelDir,modellist[i]);
			//wi.run();
			wr.init(mfccList,curdir,hmmName,modellist[i],scoredLabelDir,modellist[i],"");
			wr.run();
		}
	}

	WrapHHEd we;
	std::string t1=tmpDir+"/"+"cmd.hed";
	std::string t2=tmpDir+"/"+"hmmlist.hed";
	for(i=0;i<nsl;i++)
	{
		vec.clear();vec.push_back(modellist[i]);
		for(j=2;j<ns[i];j++)
		{
			writeHHEdFile(1,tmpDir+"/"+"cmd.hed",tmpDir+"/"+"hmmlist.hed",j,coughMix,vec);
			hmmName=outputDir+"/"+(modellist[i]+".hmm");
			we.init(hmmName,outputDir,t1,t2);
			we.run();

			wr.init(mfccList,outputDir,hmmName,modellist[i],scoredLabelDir,modellist[i],"");
			wr.run();
		}
	}
	remove(tmpDir.c_str());
}
void CoughTrain::writeCoughProtoFile(std::string tar,std::string tarKind,int numState,int vecsize,int nummixes)
{
	int i,j,k;
	FILE *fp=fopen(tar.c_str(),"w");
	fprintf(fp,"~o <VecSize> %d <%s>\n",vecsize,tarKind.c_str());
	fprintf(fp,"~h \"cough%d\"\n",numState);
	fprintf(fp,"<BeginHMM>\n");
	fprintf(fp,"<NumStates> %d\n",numState);
	for(i=2;i<numState;i++)
	{
		fprintf(fp,"<State> %d\n",i);
		fprintf(fp,"<NumMixes> %d\n",nummixes);
		for(j=1;j<=nummixes;j++)
		{
			fprintf(fp,"<Mixture> %d %f\n",j,1.0);
			fprintf(fp,"<Mean> %d\n",vecsize);
			for(k=1;k<vecsize;k++)
			{
				fprintf(fp,"%f ",0.0);
			}
			fprintf(fp,"%f\n",0.0);

			fprintf(fp,"<Variance> %d\n",vecsize);
			for(k=1;k<vecsize;k++)
			{
				fprintf(fp,"%f ",0.0);
			}
			fprintf(fp,"%f\n",0.0);
		}
	}
	fprintf(fp,"<TransP> %d\n",numState);
	double **mat=new double*[numState];
	for(i=0;i<numState;i++)
	{
		mat[i]=new double[numState];
		for(j=0;j<numState;j++)
			mat[i][j]=0.0;
	}
	mat[0][1]=1.0;
	for(i=1;i<numState-1;i++)
	{
		mat[i][i]=0.1;
		mat[i][i+1]=0.9;
	}
	for(i=0;i<numState;i++)
	{
		for(j=0;j<numState-1;j++)
		{
			fprintf(fp,"%lf ",mat[i][j]);
		}
		fprintf(fp,"%lf\n",mat[i][j]);
	}
	for(i=0;i<numState;i++)
		delete mat[i];
	delete mat;
	fprintf(fp,"<EndHMM>\n");
	fflush(fp);
	fclose(fp);
}
void CoughTrain::updateLabels(std::string tarDir,std::string srcDir,std::string scoreDir,std::string elist)
{
	FILE *ep=fopen(elist.c_str(),"r");
	char buf[1024];
	std::vector<std::string> vecSrc;
	while(fgets(buf,1024,ep)!=NULL)
	{
		std::string cs;
		trimStr(buf);
		ss<<buf;
		ss>>cs;
		ss.clear();
		cs=getFileNameWithOutExt(cs);
		vecSrc.push_back(cs);
	}
	fclose(ep);

	unsigned int i,j;
	std::vector<std::string> vecScore;
	travelDir(scoreDir,"score",&vecScore);
	
	FILE *fs=NULL,*ft=NULL;
	FILE **fa;
	fa=new FILE* [vecScore.size()];
	std::vector<std::string> vecFlag;
	for(i=0;i<vecScore.size();i++)
	{
		fa[i]=fopen(vecScore[i].c_str(),"r");
		vecFlag.push_back(getFileNameWithOutExt(vecScore[i]));
	}
	
	int srcCount=0;
	while(1)
	{
		double scoreF=-1.0E10;
		double tmp;
		double res=scoreF;
		int idx=0;
		for(j=0;j<vecScore.size();j++)
		{
			if(feof(fa[j])||fscanf(fa[j],"%lf",&tmp)==-1)
				break;
			if(tmp>res)
			{
				res=tmp;
				idx=j;
			}
		}
		if(j==vecScore.size())
		{
			double st,ed;
			char sflag[20];
			if(fs==NULL||feof(fs)||fscanf(fs,"%lf\t%lf\t%s",&st,&ed,sflag)==-1)
			{
				if(fs!=NULL)
				{
					fclose(fs);
					fclose(ft);
				}
				std::string sp=srcDir+"/"+vecSrc[srcCount]+"."+labExt;
				std::string tp=tarDir+"/"+vecSrc[srcCount]+"."+labExt;
				srcCount++;

				fs=fopen(sp.c_str(),"r");
				ft=fopen(tp.c_str(),"w");
				fscanf(fs,"%lf\t%lf\t%s",&st,&ed,sflag);
			}
			fprintf(ft,"%.0lf\t%.0lf\t%s\n",st,ed,vecFlag[idx].c_str());
		}
		else
		{
			fclose(fs);
			fclose(ft);
			break;
		}
	}

	for(i=0;i<vecScore.size();i++)
	{
		fclose(fa[i]);
	}
	delete[] fa;
}
