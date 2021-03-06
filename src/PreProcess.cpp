#pragma once
#include "glb.h"
#include "PreProcess.h"
#include "Util.h"
#include "htks.h"
#include "HashTable.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"



std::string PreProcess::getEmptyList()
{
	return emptyList;
}
std::string PreProcess::getLabeledList()
{
	return labeledList;
}
std::string PreProcess::getMfccList()
{
	return mfccList;
}
std::string PreProcess::getSplitedLabelDir()
{
	return splitedLabelDir;
}
void PreProcess::init(std::string inWorkingDir,std::string inAudioDir,std::string inLabelDir)
{
	cut_map.clear();
	workingDir=inWorkingDir;
	audioDir=inAudioDir;
	mfccDir=workingDir+"/"+"mfccs";
	mfccList=workingDir+"/"+"mfcclist.txt";
	tmpDir=workingDir+"/"+"tmp";
	labelDir=inLabelDir;
	splitedLabelDir=workingDir+"/"+"splitedLabs";
	emptyList=workingDir+"/"+"emptylist.txt";
	labeledList=workingDir+"/"+"labeledlist.txt";
}
void PreProcess::run(int frame_th,int low_th,bool tg,bool demp3)
{
	createEmptyDir(workingDir);
	createEmptyDir(mfccDir);
	createEmptyDir(splitedLabelDir);
	createEmptyDir(tmpDir);
	
	int i,j;

	writeConfFile(workingDir+"/"+"analysis.conf",gTarKind,320000.0,160000.0);
	std::vector<std::string> vecAudio;

	FILE* inf=fopen(((workingDir+"/"+"cut.inf")).c_str(),"w");
	std::string confPath=workingDir+"/"+confName;
	char savePath[1024];
	FILE* savePtr=fopen(confPath.c_str(),"r");
	fscanf(savePtr,"%s",savePath);
	fclose(savePtr);

	if(demp3)
	{
		travelDir(audioDir,"mp3",&vecAudio);
		printf("total 10-min %d\n",vecAudio.size());
		makeupTg(audioDir,labelDir,"mp3",tgExt);

		for(i=0;i<vecAudio.size();i++)
		{
			printf("%d/%d\n",i+1,vecAudio.size());
			/*--------------------*/
//			Mp3Decoder md;
//			md.init((char*)vecAudio[i].c_str());
//			std::string pcmPath="./tmp.pcm";
//			vecAudio[i]="./"+getFileNameWithOutExt(vecAudio[i])+".wav";
//			md.run(pcmPath,0,-1);
//			md.writeWavHeader(pcmPath,vecAudio[i],md.nchannel,md.dataSize);
			std::string mp3Path=vecAudio[i];
			vecAudio[i]="./"+getFileNameWithOutExt(vecAudio[i])+".wav";
			//printf("%s %s\n",mp3Path.c_str(),vecAudio[i].c_str());
			char cmd[1024];
			//sprintf(cmd,"mpg123 -q -w %s %s",vecAudio[i].c_str(),mp3Path.c_str());
			sprintf(cmd,"madplay -m -R 16000 -o %s %s",vecAudio[i].c_str(),mp3Path.c_str());
			system(cmd);
			/*--------------------*/
			int maxSplit=~(1<<31);
			int curSec=0;
			wavFormat wf;
			wf.audioOpen(vecAudio[i]);
			std::string fileName=getFileNameWithOutExt(vecAudio[i]);
			for(j=0;j<maxSplit;j++)
			{
				std::string fpath;
				ss<<fileName<<"_"<<j<<".wav";
				ss>>fpath;
				ss.clear();
				fpath=std::string(savePath)+"/"+fpath;
				//fpath=workingDir+"/"+fpath;
				std::string mfccPath=mfccDir+"/"+getFileNameWithOutExt(fpath)+".mfcc";
				std::string confPath=workingDir+"/"+"analysis.conf";
				int isTail=wf.subAudio(fpath,curSec,frame_th);
				if(isTail!=0)
				{
					remove(fpath.c_str());
					break;
				}
				WrapHCopy wh;
				wh.init(confPath,fpath, mfccPath);
				wh.run();	
				//remove(fpath.c_str());
				curSec+=frame_th;	
			}
			wf.audioClose();
			int snum=j;
			//make .inf
			//FILE* inf=fopen(((workingDir+"/"+fileName+".inf")).c_str(),"w");
			//fprintf(inf,"%d\t%d\n",frame_th*1000,snum);
			fprintf(inf,"%s\t%d\t%d\n",fileName.c_str(),frame_th*1000,snum);
			cut_map.insert(std::make_pair(fileName,cut_node(frame_th*1000,snum)));
			//fclose(inf);
			/*--------------------*/
			//remove(pcmPath.c_str());
			remove(vecAudio[i].c_str());
		}
	}
	else/*process wav format*/
	{
		travelDir(audioDir,"wav",&vecAudio);
		makeupTg(audioDir,labelDir,"wav",tgExt);
	
		for(i=0;i<vecAudio.size();i++)
		{
			int maxSplit=~(1<<31);
			int curSec=0;
			wavFormat wf;
			wf.audioOpen(vecAudio[i]);
			std::string fileName=getFileNameWithOutExt(vecAudio[i]);
			for(j=0;j<maxSplit;j++)
			{
				std::string fpath;
				ss<<fileName<<"_"<<j<<".wav";
				ss>>fpath;

				ss.clear();

				fpath=std::string(savePath)+"/"+fpath;

//				fpath=workingDir+"/"+fpath;
				std::string mfccPath=mfccDir+"/"+getFileNameWithOutExt(fpath)+".mfcc";
				std::string confPath=workingDir+"/"+"analysis.conf";
				int isTail=wf.subAudio(fpath,curSec,frame_th);
				if(isTail!=0)
				{
					remove(fpath.c_str());
					break;
				}
				WrapHCopy wh;
				wh.init(confPath,fpath, mfccPath);
				wh.run();
//				remove(fpath.c_str());
				curSec+=frame_th;	
			}
			wf.audioClose();
			int snum=j;
			fprintf(inf,"%s\t%d\t%d\n",fileName.c_str(),frame_th*1000,snum);
			cut_map.insert(std::make_pair(fileName,cut_node(frame_th*1000,snum)));
		}
	}
	fclose(inf);

	//make mfcclist.txt
	std::vector<std::string> vec;
	travelDir(mfccDir,"mfcc",&vec);
	writeListFile(mfccList,vec);

	vec.clear();
	if(tg)
	{
		travelDir(labelDir,tgExt,&vec);
		for(j=0;j<vec.size();j++)
		{
			tgToLabel(vec[j],tmpDir);
		}
		vec.clear();
		travelDir(tmpDir,"lab",&vec);
	}
	else
		travelDir(labelDir,"lab",&vec);

	FILE* ep=fopen(emptyList.c_str(),"w");
	for(int i=0;i<vec.size();i++)
	{
		//make empty list
		std::vector<int> emptyVec=splitLabel(vec[i],splitedLabelDir,workingDir);
		for(j=0;j<emptyVec.size();j++)
			fprintf(ep,"%s_%d%s\n",(mfccDir+"/"+getFileNameWithOutExt(vec[i])).c_str(),emptyVec[j],("."+mfccExt).c_str());	
	}
	fclose(ep);
	writeLabeledList(labeledList,mfccList,emptyList);
	remove(tmpDir.c_str());
}
void PreProcess::writeLabeledList(std::string tar,std::string srcComplete,std::string srcEmpty)
{
	FILE *ep=fopen(srcEmpty.c_str(),"r");
	FILE *fp=fopen(srcComplete.c_str(),"r");
	FILE *tp=fopen(tar.c_str(),"w");
	HashTable ht;
	std::string cstmp;
	char buf[1024];
	while(fgets(buf,1024,ep)!=NULL)
	{
		trimStr(buf);
		cstmp=getFileNameWithOutExt(std::string(buf));
		ht.insert((char*)(cstmp.c_str()));
	}
	while(fgets(buf,1024,fp)!=NULL)
	{
		trimStr(buf);
		cstmp=getFileNameWithOutExt(std::string(buf));
		if(!ht.check((char*)(cstmp.c_str())))
		{
			fprintf(tp,"%s\n",buf);
		}
	}
	fclose(tp);
	fclose(fp);
	fclose(ep);
}

std::vector<int> PreProcess::splitLabel(std::string src,std::string tarDir,std::string infDir)
{
	FILE* srcf=fopen(src.c_str(),"r");
	FILE* tarf=NULL;
	int ab=0;
	double tts,tte;
	double unitScale=1.0e4;
	char flag[10];
	std::string fileName=getFileNameWithOutExt(src);
	int segCount;
	double interval;
	std::vector<int> ret;
	std::map<std::string,cut_node>::iterator itr=cut_map.find(fileName);
	interval=((cut_node)itr->second).interval;
	segCount=((cut_node)itr->second).count;
//	FILE* inf=fopen((infDir+"/"+fileName+".inf").c_str(),"r");
//	fscanf(inf,"%lf\t%d",&interval,&segCount);
//	fclose(inf);

	for(int i=0;i<segCount;i++)
	{
		std::string tarName;
		ss<<fileName<<"_"<<i<<".lab";
		ss>>tarName;
		ss.clear();
		tarf=fopen((tarDir+"/"+tarName).c_str(),"w");
		bool empty=true;
		double ts=i*interval*unitScale;
		double te=(i+1)*interval*unitScale;
		while(1)
		{
			int brk=0;
			if(ab==0)
			{
				if(fscanf(srcf,"%lf\t%lf\t%s",&tts,&tte,flag)!=-1)
					ab=1;
				else if(feof(srcf))
					ab=-1;
				else
					printf("error in spliting label file\n");//some error
			}
			else if(ab==1)
			{
				if(tts>=te)
					brk=1;
				else if(tte>te||tts<ts||tte<ts)
				{
					ab=0;
				}
				else
				{
					fprintf(tarf,"%.0lf\t%.0lf\t%s\n",tts-ts,tte-ts,flag);
					empty=false;
					ab=0;
				}
			}
			else
			{
				brk=1;
			}
			if(brk)
				break;
			/*
			int brk=0;
			if(ab==0)
			{
				if(fscanf(srcf,"%lf\t%lf\t%s",&tts,&tte,flag)!=-1)
					ab=1;
				else if(feof(srcf))
					ab=-1;
				else
					;//some error
			}
			else if(ab==1)
			{	
				if(te<=tts)
				{
					brk=1;
				}
				else if(te<tte)
				{
					fprintf(tarf,"%.0lf\t%.0lf\t%s\n",tts-ts,te-ts,flag);
					empty=false;
					tts=te;
					brk=1;
				}
				else
				{
					fprintf(tarf,"%.0lf\t%.0lf\t%s\n",tts-ts,tte-ts,flag);
					empty=false;
					ab=0;
				}
			}
			else
			{
				brk=1;
			}
			if(brk)
				break;
			*/
		}
		if(empty)
		{
			fprintf(tarf,"0\t0\tNONE\n");
			ret.push_back(i);
		}
		fclose(tarf);
	}
	fclose(srcf);
	return ret;
}
void PreProcess::makeupTg(std::string audioDir,std::string tgDir,std::string audioExt,std::string tgExt)
{
	int i;
	std::vector<std::string> vec;
	HashTable ht;
	travelDir(tgDir,tgExt,&vec);
	for(i=0;i<vec.size();i++)
		ht.insert((char*)getFileNameWithOutExt(vec[i]).c_str());

	vec.clear();
	travelDir(audioDir,audioExt,&vec);
	for(i=0;i<vec.size();i++)
	{
		std::string fileName=getFileNameWithOutExt(vec[i]);
		if(!ht.check((char*)fileName.c_str()))
		{
			std::string tarTg=tgDir+"/"+fileName+"."+tgExt;
			FILE *fp=fopen(tarTg.c_str(),"w");
			fprintf(fp,"NO COUGH\n");
			fclose(fp);
		}
	}
	
}
