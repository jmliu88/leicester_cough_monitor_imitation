#include "glb.h"
#include "FillerTrain.h"
#include "Util.h"
#include "htks.h"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
pthread_mutex_t FillerTrain::mutex_count=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t FillerTrain::mutex_data=PTHREAD_MUTEX_INITIALIZER;
std::string FillerTrain::getOutputDir()
{
	return outputDir;
}
std::string FillerTrain::getFillerLabelDir()
{
	return fillerLabelDir;
}
void FillerTrain::init(std::string inWorkingDir,std::string inMfccList)
{
	workingDir=inWorkingDir;
	mfccList=inMfccList;
	tmpDir=workingDir+"/"+"tmp";
	fillerLabelDir=workingDir+"/"+"s1fLabs";
	modelDir=workingDir+"/"+"s1model";
	//outputDir=modelDir+"/"+"filler";
}
void* FillerTrain::mul_thread_train_filler(void *arg)
{
	std::string itemp;
	std::string mflag="filler";
	WrapHInit wi;
	WrapHRest wr;

	struct train_info data_ptr=*reinterpret_cast<struct train_info*>(arg);
	std::string mfccList=*data_ptr.ptr_mfccList;
	std::string fillerLabelDir=*data_ptr.ptr_fillerLabelDir;
	std::string curdir=*data_ptr.ptr_curdir;
	int filler_counter;
	int filler_total=*data_ptr.ptr_filler_total;
	pthread_mutex_unlock(&FillerTrain::mutex_data);
	while(true)
	{
		pthread_mutex_lock(&FillerTrain::mutex_count);
		filler_counter=*data_ptr.ptr_filler_counter;
		ss<<filler_counter;
		ss>>itemp;
		ss.clear();

		if(filler_counter>=filler_total)
		{
			std::cout<<"-"<<pthread_self()<<" "<<filler_counter<<std::endl;
			pthread_mutex_unlock(&FillerTrain::mutex_count);
			break;
		}
		else
		{
			std::cout<<"+"<<pthread_self()<<" "<<filler_counter<<std::endl;
			(*data_ptr.ptr_filler_counter)++;
			pthread_mutex_unlock(&FillerTrain::mutex_count);
			std::string srcM=curdir+"/"+mflag+itemp+"."+hmmExt;
			itemp=mflag+itemp;

//			std::cout<<pthread_self()<<"\t"<<filler_counter<<"\t"<<filler_total<<std::endl;
//			std::cout<<mfccList<<"\t"<<fillerLabelDir<<"\t"<<curdir<<std::endl;
			//std::cout<<"#"<<pthread_self()<<"\n"<<mfccList<<"\n"<<curdir<<"\n"<<srcM<<"\n"<<itemp<<"\n"<<fillerLabelDir<<"\n";
//			wr.init(mfccList,curdir,srcM,itemp,fillerLabelDir,itemp,"");
//			wr.run();
			char cmdstr[1024];
			sprintf(cmdstr,"HRest -m 1 -S %s -M %s -H %s -l %s -L %s %s",mfccList.c_str(),curdir.c_str(),srcM.c_str(),itemp.c_str(),fillerLabelDir.c_str(),itemp.c_str());
			//printf("%s\n",cmdstr);
			system(cmdstr);
		}
	}
	return NULL;
}
void FillerTrain::run(int repTimes,double var)
{
	createEmptyDir(workingDir);
	createEmptyDir(tmpDir);
	createEmptyDir(modelDir);
	createEmptyDir(fillerLabelDir);
	//createEmptyDir(outputDir);
	int i;
	int p2[10];
	p2[0]=1;
	for(i=1;i<sizeof(p2)/sizeof(int);i++)
		p2[i]=p2[i-1]*2;
	std::string hmmName;
	std::string itemp;
	std::string curdir,predir;
	std::string mflag="filler";
	int iterCount=0;
	
	//predir=globalInf;
	ss<<iterCount++;
	ss>>itemp;
	ss.clear();
	curdir=modelDir+"/"+"r"+itemp;
	hmmName=curdir+"/"+mflag+itemp+"."+hmmExt;
	createEmptyDir(curdir);
	writeFillerProtoFile(hmmName,gTarKind,fillerStateNum,39,1);

	//====get global mean and var
	WrapHCompV wc;
	wc.init(mfccList,curdir,hmmName,mflag+itemp);
	wc.run();
	//====end

	int it=0;
	WrapHParse wp;
	WrapHVite wv;
	WrapHRest wr;
	for(it=0;it<repTimes;it++)
	{
		predir=curdir;
		ss<<iterCount++;
		ss>>itemp;
		ss.clear();
		curdir=modelDir+"/"+"r"+itemp;
		createEmptyDir(curdir);		
		for(i=0;i<p2[it];i++)
		{
			ss<<i;
			ss>>itemp;
			ss.clear();
			std::string srcM=predir+"/"+mflag+itemp+"."+hmmExt;

			ss<<i*2;
			ss>>itemp;
			ss.clear();
			std::string tarM=curdir+"/"+mflag+itemp+"."+hmmExt;
			updateFillerHmm(srcM,tarM,i*2,0.01,39);

			ss<<i*2+1;
			ss>>itemp;
			ss.clear();
			tarM=curdir+"/"+mflag+itemp+"."+hmmExt;
			updateFillerHmm(srcM,tarM,i*2+1,-0.01,39);
		}
		//make dict gram net mmf
		std::string tdict=tmpDir+"/dict.txt";
		std::string tgram=tmpDir+"/gram.txt";
		std::string tnet=tmpDir+"/net.txt";
		std::string tlist=tmpDir+"/hmmlist.txt";
		std::string tmmf=tmpDir+"/hmm.mmf";
		writeDict(tdict,mflag,0,p2[it]*2);
		writeGram(tgram,mflag,0,p2[it]*2);
		wp.init(tgram,tnet);
		wp.run();

		std::vector<std::string> vecHmm;
		travelDir(curdir,"hmm",&vecHmm);
		writeHmmListAndDefFile(tlist,tmmf,vecHmm,false);
		//reco
		wv.init(curdir,"",fillerLabelDir,tnet,tdict,tlist,mfccList);
		wv.run();

		//retrain
		//multithread initialization
		pthread_t *t=new pthread_t[thread_num];
		filler_counter=0;
		filler_total=p2[it]*2;
		FillerTrain::mutex_count=PTHREAD_MUTEX_INITIALIZER;
		FillerTrain::mutex_data=PTHREAD_MUTEX_INITIALIZER;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		for(i=0;i<thread_num;i++)
		{
			pthread_mutex_lock(&FillerTrain::mutex_data);
			struct train_info data_ptr;
			data_ptr.ptr_mfccList=&mfccList;
			data_ptr.ptr_fillerLabelDir=&fillerLabelDir;
			data_ptr.ptr_curdir=&curdir;
			data_ptr.ptr_filler_counter=&filler_counter;
			data_ptr.ptr_filler_total=&filler_total;
			pthread_create(t+i,&attr,mul_thread_train_filler,reinterpret_cast<void*>(&data_ptr));
		}
		pthread_attr_destroy(&attr);
		for(i=0;i<thread_num;i++)
		{
			pthread_join(t[i],NULL);
		}
		delete[] t;

//		WrapHInit wi;
//		for(i=0;i<p2[it]*2;i++)
//		{
//			//printf("=================%d_%d\n",it,i);
//			ss<<i;
//			ss>>itemp;
//			ss.clear();
//			std::string srcM=curdir+"/"+mflag+itemp+"."+hmmExt;
//			itemp=mflag+itemp;
//
//			//wi.init(mfccList,curdir,srcM,itemp,fillerLabelDir,itemp);
//			//wi.run();
//
//			wr.init(mfccList,curdir,srcM,itemp,fillerLabelDir,itemp,"");
//			wr.run();
//		}
	}
	outputDir=curdir;

	remove(tmpDir.c_str());
}
void FillerTrain::updateFillerHmm(std::string src,std::string tar,int idx,double var,int vecsize)
{
	std::string fptmp;
	std::string rptmp;
	std::string cstmp;
	char strtmp[1024];
	FILE *fp=fopen(tar.c_str(),"w");
	FILE *rp=fopen(src.c_str(),"r");
	bool mflag=false;
	while(true)
	{
		if(mflag)
			break;
		fgets(strtmp,1024,rp);
		cstmp=strtmp;
		if(cstmp.find("~h")!=std::string::npos)
		{
			fprintf(fp,"~h \"filler%d\"\n",idx);
			break;
		}
		else
		{
			fputs(strtmp,fp);
		}
	}
	int i;
	while(1)
	{
		if(mflag)
		{
			double dtmp;
			for(i=0;i<vecsize;i++)
			{
				fscanf(rp,"%lf",&dtmp);
				if(i==vecsize-1)
					fprintf(fp,"%e\n",dtmp+var);
				else
					fprintf(fp,"%e ",dtmp+var);
			}	
			mflag=false;
			fgetc(rp);
		}
		else
		{
			fgets(strtmp,1024,rp);
			if(feof(rp))
				break;
			cstmp=strtmp;
			if(cstmp.find("<MEAN>")!=std::string::npos)
				mflag=true;
			fputs(strtmp,fp);
		}
	}
	fclose(rp);
	fflush(fp);
	fclose(fp);
}

void FillerTrain::writeFillerProtoFile(std::string tar,std::string tarKind,int numState,int vecsize,int nummixes)
{
	std::string cstmp;
	FILE *fp=fopen(tar.c_str(),"w");
	//FILE *rp=fopen(ref.c_str(),"r");
	//double meanTmp[256];
	//double varTmp[256];
	int vs=vecsize;
	int i,j,k;
	bool mflag=false,vflag=false;
	//while(true)
	//{
	//	fgets(buf,1024,rp);
	//	cstmp=buf;
	//	if(cstmp.find("<MEAN>")!=std::string::npos)
	//	{
	//		mflag=true;
	//		break;
	//	}
	//}
	//for(i=0;i<vs;i++)
	//	fscanf(rp,"%lf",&meanTmp[i]);
	//while(true)
	//{
	//	fgets(buf,1024,rp);
	//	cstmp=buf;
	//	if(cstmp.find("<VARIANCE>")!=std::string::npos)
	//	{
	//		vflag=true;
	//		break;
	//	}
	//}
	//for(i=0;i<vs;i++)
	//	fscanf(rp,"%lf",&varTmp[i]);
	//fclose(rp);

	fprintf(fp,"~o <VecSize> %d <%s>\n",vecsize,tarKind.c_str());
	fprintf(fp,"~h \"filler%d\"\n",0);
	fprintf(fp,"<BeginHMM>\n");
	fprintf(fp,"<NumStates> %d\n",numState);
	for(i=2;i<numState;i++)
	{
		fprintf(fp,"<State> %d\n",i);
		fprintf(fp,"<NumMixes> %d\n",nummixes);
		for(j=1;j<=nummixes;j++)
		{
			fprintf(fp,"<Mixture> %d %f\n",j,1.0);
			fprintf(fp,"<MEAN> %d\n",vecsize);
			for(k=1;k<vecsize;k++)
			{
				fprintf(fp,"%e ",0.0);//fprintf(fp,"%e ",meanTmp[k-1]);
			}
			fprintf(fp,"%e\n",0.0);//fprintf(fp,"%e\n",meanTmp[k-1]);

			fprintf(fp,"<VARIANCE> %d\n",vecsize);
			for(k=1;k<vecsize;k++)
			{
				fprintf(fp,"%e ",0.0);//fprintf(fp,"%e ",varTmp[k-1]);
			}
			fprintf(fp,"%e\n",0.0);//fprintf(fp,"%e\n",varTmp[k-1]);
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
		mat[i][i+1]=0.5;
		if(i+2<numState)
			mat[i][i+2]=0.4;
		else
			mat[i][i+1]+=0.4;
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
