#pragma once
#include "glb.h"
#include "Util.h"
#include "HashTable.h"
#include <map>
#include "mfccReader.h"
#include <algorithm>
#include <vector>
#include "string.h"
#include "stdio.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "dirent.h"
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
std::vector<std::string> readListFile(std::string tar)
{
	FILE* fp=fopen(tar.c_str(),"r");
	std::vector<std::string> vec;
	char buf[128];
	while(fgets(buf,128,fp)!=NULL)
	{
		trimStr(buf);
		std::string tmp=buf;
		vec.push_back(tmp);
	}
	fclose(fp);
	return vec;
}
void combineListFile(std::string src1,std::string src2)
{
	FILE *f1=fopen(src1.c_str(),"a");
	FILE *f2=fopen(src2.c_str(),"r");
	char buf[128];
	while(fgets(buf,128,f2)!=NULL)
	{
		fputs(buf,f1);
	}
	fclose(f1);
	fclose(f2);
}
void trimStr(char *buf)
{
	int len=strlen(buf);
	int st,et;
	int cnt=0;
	for(int i=0;i<len;i++)
	{
		if(buf[i]!=' '&&buf[i]!='\t'&&buf[i]!='\n'&&buf[i]!='\r'&&buf[i]!='\r\n')
		{
			st=i;
			break;
		}
	}
	for(int i=len-1;i>=0;i--)
	{
		if(buf[i]!=' '&&buf[i]!='\t'&&buf[i]!='\n'&&buf[i]!='\r')
		{
			et=i;
			break;
		}
	}
	for(int i=st;i<=et;i++)
		buf[cnt++]=buf[i];
	buf[cnt]=0;
}
void travelDir(std::string rootDir,std::string suffix,std::vector<std::string> *vec)
{
	DIR *dir;
	struct stat buf;
	struct dirent *file;
	dir=opendir(rootDir.c_str());
	while((file=readdir(dir))!=NULL)
	{
		if(DT_DIR&file->d_type)
		{
			if(strcmp(".",file->d_name)==0||strcmp("..",file->d_name)==0)
				continue;
			char sub_path[1024];
			sprintf(sub_path,"%s/%s",rootDir.c_str(),file->d_name);
			travelDir(sub_path,suffix,vec);
		}
		else
		{
			std::string stmp(file->d_name);
			int idx=stmp.find('.');
			stmp=stmp.substr(idx+1,stmp.length()-idx-1);
			if(idx!=std::string::npos)
			{
				if(stmp.compare(suffix)==0)
					vec->push_back(rootDir+"/"+file->d_name);
			}
		}
	}
	closedir(dir);
	/*
	WIN32_FIND_DATA findFileData;
	std::string tpf="\\*."+suffix;
	HANDLE hFind=::FindFirstFile((rootDir+tpf).c_str(), &findFileData);
	if(INVALID_HANDLE_VALUE == hFind)
    {
		fprintf(stderr,"travel failed..\n");
		return;
	}
	while(TRUE)
    {
		if(findFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			if(findFileData.cFileName[0]!='.')
			{
				travelDir(rootDir+"/"+findFileData.cFileName+tpf,suffix,vec);
			}
		}
		else
		{
			std::string stmp=findFileData.cFileName;
			int idx=stmp.find('.');
			if(idx!=std::string::npos)
			{
				stmp=stmp.substr(idx+1,stmp.length()-idx-1);
				if(stmp.compare(suffix)==0)
					vec->push_back(rootDir+"/"+findFileData.cFileName);
			}
		}
		if(!FindNextFile(hFind,&findFileData))
              break;
	}
	FindClose(hFind); 
	*/
}


void createEmptyDir(std::string dir)
{
	struct stat buf;
	if(stat(dir.c_str(),&buf)!=0)
		mkdir(dir.c_str(),S_IRWXU|S_IRWXG|S_IRWXO);
	/*
	if(GetFileAttributes(dir.c_str())!=-1)
		DeleteFile(dir.c_str());
	CreateDirectory(dir.c_str(),NULL);
	*/
}
std::string getFileNameWithExt(std::string src)
{
	int t=src.rfind('/');
	if(t==-1)
		return src;
	else
	{
		int s=t+1;
		int l=src.length()-s;
		return src.substr(s,l);
	}
}
std::string getFileNameWithOutExt(std::string src)
{
	std::string ret=getFileNameWithExt(src);
	int t=ret.find('.');
	if(t==-1)
		return ret;
	else
		return ret.substr(0,t);
}

std::string convertPathFormat(std::string src)
{
	char p[1024];
	unsigned int i;
	for(i=0;i<src.length();i++)
	{
		char temp=src[i];
		if(temp=='\\')
			temp='/';
		p[i]=temp;
	}
	p[i]=0;
	return std::string(p);
}
void writeListFile(std::string tar,std::vector<std::string> &vec)
{
	FILE* fp=fopen(tar.c_str(),"w");
	for(unsigned int i=0;i<vec.size();i++)
	{
		fprintf(fp,"%s\n",vec[i].c_str());
	}
	fclose(fp);
}

void writeConfFile(std::string tar,std::string tarKind,double winSize,double tarRate)
{
	FILE *fp=fopen(tar.c_str(),"w");
	fprintf(fp,"SOURCEFORMAT = %s\n","WAV");
	fprintf(fp,"TARGETKIND = %s\n",tarKind.c_str());
	fprintf(fp,"WINDOWSIZE = %f\n",winSize);
	fprintf(fp,"TARGETRATE = %f\n",tarRate);
	fprintf(fp,"NUMCEPS = %d\n",12);
	fprintf(fp,"USEHAMMING = %s\n","T");
	//fprintf(fp,"PREEMCOEF = %f\n",0.97);
	fprintf(fp,"NUMCHANS = %d\n",16);
	fprintf(fp,"CEPLIFTER = %d\n",22);
	fprintf(fp,"SAVEWITHCRC = F\n");
	fprintf(fp,"SAVECOMPRESSED = F\n");
	/*
	if(stereo!="A")
		fprintf(fp,"STEREOMODE = %s\n",stereo.c_str());
	*/
	fflush(fp);
	fclose(fp);
}

void writeHHEdFile(int singleState,std::string cmdTar,std::string hmmlistTar,int ns,int mixnum,std::vector<std::string> mName)
{
	unsigned int i;
	FILE *fp=fopen(cmdTar.c_str(),"w");
	for(i=0;i<mName.size();i++)
	{
		if(!singleState)
			fprintf(fp,"MU %d {%s.state[1-%d].mix}\n",mixnum,mName[i].c_str(),ns);
		else
			fprintf(fp,"MU %d {%s.state[%d].mix}\n",mixnum,mName[i].c_str(),ns);
	}
	fflush(fp);
	fclose(fp);
	fp=fopen(hmmlistTar.c_str(),"w");
	for(i=0;i<mName.size();i++)
		fprintf(fp,"%s\n",mName[i].c_str());
	fflush(fp);
	fclose(fp);
}

void writeGram(std::string tar,std::vector<std::string> vec)
{
	unsigned int i;
	FILE *fp=fopen(tar.c_str(),"w");
	fprintf(fp,"( < ");
	for(i=0;i<vec.size();i++)
	{
		std::string tmp=getFileNameWithOutExt(vec[i]);
		fprintf(fp," %s ",tmp.c_str());
		if(i!=(vec.size()-1))
			fputc('|',fp);
	}
	fprintf(fp," > ) ");
	fclose(fp);
}

void writeGramBinary(std::string tar,std::vector<std::string> vec)
{
	unsigned int i;
	FILE *fp=fopen(tar.c_str(),"w");
	fprintf(fp,"( ");
	for(i=0;i<vec.size();i++)
	{
		std::string tmp=getFileNameWithOutExt(vec[i]);
		fprintf(fp," %s ",tmp.c_str());
		if(i!=(vec.size()-1))
			fputc('|',fp);
	}
	fprintf(fp," ) ");
	fclose(fp);
}

void writeGram(std::string tar,std::string hmmName,int st,int ed)
{
	int i;
	FILE *fp=fopen(tar.c_str(),"w");
	fprintf(fp,"( < ");
	for(i=st;i<ed;i++)
	{
		fprintf(fp," %s%d ",hmmName.c_str(),i);
		if(i!=(ed-1))
			fputc('|',fp);
	}
	fprintf(fp," > ) ");
	fclose(fp);
}
void writeDict(std::string tar,std::vector<std::string> vec)
{
	FILE *fp=fopen(tar.c_str(),"w");
	unsigned int i;
	for(i=0;i<vec.size();i++)
	{
		std::string tmp=getFileNameWithOutExt(vec[i]);
		fprintf(fp,"%s [%s] %s\n",tmp.c_str(),tmp.c_str(),tmp.c_str());
	}
	fclose(fp);
}
void writeDict(std::string tar,std::string hmmName,int st,int ed)
{
	FILE *fp=fopen(tar.c_str(),"w");
	int i;
	for(i=st;i<ed;i++)
	{
		std::string temp;
		ss<<hmmName<<i;
		ss>>temp;
		ss.clear();
		fprintf(fp,"%s [%s] %s\n",temp.c_str(),temp.c_str(),temp.c_str());
	}
	fclose(fp);
}
void writeHmmListAndDefFile(std::string tarList,std::string tarDef,std::vector<std::string> vecHmm,bool mmf)
{	
	FILE* fp=fopen(tarList.c_str(),"w");
	for(unsigned int i=0;i<vecHmm.size();i++)
	{
		fprintf(fp,"%s\n",getFileNameWithOutExt(vecHmm[i]).c_str());
	}
	fclose(fp);

	if(mmf)
	{
		char buf[1024];
		std::string cstmp;
		fp=fopen(tarDef.c_str(),"w");
		for(unsigned int i=0;i<vecHmm.size();i++)
		{
			FILE *rp=fopen(vecHmm[i].c_str(),"r");
			bool flag=false;
			if(i==0)
				flag=true;
			while(true)
			{
				fgets(buf,1024,rp);
				if(flag==false)
				{
					cstmp=buf;
					if(cstmp.find("~h")==std::string::npos)
						continue;
					else
						flag=true;
				}
				if(feof(rp))
					break;
				fputs(buf,fp);
			}
			fclose(rp);
		}
		fclose(fp);
	}
}

void tgToLabel(std::string src,std::string tarDir)
{
	std::string bas=".lab";
	std::string tar=tarDir+"/"+getFileNameWithOutExt(src)+bas;
	FILE *sp=fopen(src.c_str(),"r");
	FILE *tp=fopen(tar.c_str(),"w");
	char buf[1024];
	char buf1[1024];
	char buf2[1024];
	int buflen=sizeof(buf)/sizeof(char);
	char cmpstr[]="intervals: size =";
	int segNum=0;
	char mflag[]="\"1\"";
	char oflag[]="cough";
	char tflag[32];
	double scale=1e7;
	std::string cstmp;
	while(fgets(buf,buflen,sp)!=NULL)
	{
		if(feof(sp)==true)
			break;
		cstmp=buf;
		if(cstmp.find(cmpstr)!=std::string::npos)
		{
			trimStr(buf);
			sscanf(buf,"intervals: size = %d",&segNum);
			break;
		}
	}
	for(int i=0;i<segNum;i++)
	{
		double st,et;
		fgets(buf,buflen,sp);
		//xmin
		fgets(buf1,buflen,sp);
		//xmax
		fgets(buf2,buflen,sp);
		//text
		fgets(buf,buflen,sp);
		trimStr(buf);
		sscanf(buf,"text = %s",tflag);
		trimStr(tflag);
		if(strcmp(tflag,mflag)==0)//if(strlen(tflag)>2)
		{
			trimStr(buf1);
			sscanf(buf1,"xmin = %lf",&st);
			trimStr(buf2);
			sscanf(buf2,"xmax = %lf",&et);
			fprintf(tp,"%.0lf\t%.0lf\t%s\n",st*scale,et*scale,oflag);
		}
	}
	fclose(sp);
	fclose(tp);
}

void copyAudioLabelPair(std::string aSrcDir,std::string lSrcDir,std::string aTarDir,std::string lTarDir,std::string labFlag)
{
	std::string bas=".mp3";
	std::vector<std::string> vec;
	travelDir(lSrcDir,labFlag,&vec);
	for(unsigned int i=0;i<vec.size();i++)
	{
		std::string fileName=getFileNameWithOutExt(vec[i]);
		char cmd[1024];
		sprintf(cmd,"cp %s %s",(aSrcDir+"/"+fileName+bas).c_str(),(aTarDir+"/"+fileName+bas).c_str());
		system(cmd);
	}
}
void combineListFile(std::string tarPath,std::string src1,std::string src2)
{
	FILE *rp;
	FILE *wp;
	wp=fopen(tarPath.c_str(),"w");
	rp=fopen(src1.c_str(),"r");
	char buf[1024];
	while(fgets(buf,1024,rp)!=NULL)
	{
		fputs(buf,wp);
	}
	fclose(rp);

	rp=fopen(src2.c_str(),"r");
	while(fgets(buf,1024,rp)!=NULL)
	{
		fputs(buf,wp);
	}
	fclose(rp);

	fclose(wp);
}
void combineLabels(std::string cLabDir,std::string fLabDir,std::string labeledlist,std::string emptylist,std::string tarDir)
{
	FILE* tp=fopen(labeledlist.c_str(),"r");
	FILE* ep=fopen(emptylist.c_str(),"r");
	char buf[1024];
	std::string cstmp;
	while(fgets(buf,1024,tp)!=NULL)
	{
		trimStr(buf);
		std::string srcDir=cLabDir;
		cstmp=getFileNameWithOutExt(std::string(buf));
		char cmd[1024];
		sprintf(cmd,"cp %s %s",(srcDir+"/"+cstmp+".lab").c_str(),(tarDir+"/"+cstmp+".lab").c_str());
		system(cmd);
		//CopyFile((srcDir+"/"+cstmp+".lab").c_str(),(tarDir+"/"+cstmp+".lab").c_str(),FALSE);
	}
	fclose(tp);
	while(fgets(buf,1024,ep)!=NULL)
	{
		trimStr(buf);
		std::string srcDir=fLabDir;
		cstmp=getFileNameWithOutExt(std::string(buf));
		char cmd[1024];
		sprintf(cmd,"cp %s %s",(srcDir+"/"+cstmp+".lab").c_str(),(tarDir+"/"+cstmp+".lab").c_str());
		system(cmd);
		//CopyFile((srcDir+"/"+cstmp+".lab").c_str(),(tarDir+"/"+cstmp+".lab").c_str(),FALSE);
	}
	fclose(ep);
	/*
	FILE* tp=fopen(trainlist.c_str(),"r");
	FILE* ep=fopen(emptylist.c_str(),"r");
	char buf[1024];
	HashTable ht;
	std::string cstmp;
	while(fgets(buf,1024,ep)!=NULL)
	{
		trimStr(buf);
		cstmp=getFileNameWithOutExt(std::string(buf));
		ht.insert((char*)cstmp.c_str());
	}

	while(fgets(buf,1024,tp)!=NULL)
	{
		trimStr(buf);
		std::string srcDir;
		cstmp=getFileNameWithOutExt(std::string(buf));
		if(ht.check((char*)cstmp.c_str()))
			srcDir=fLabDir;
		else
			srcDir=cLabDir;
		CopyFile((srcDir+"/"+cstmp+".lab").c_str(),(tarDir+"/"+cstmp+".lab").c_str(),FALSE);
	}
	fclose(tp);
	fclose(ep);
	*/
}
void evaluateResult(std::string recDir,std::string labDir,std::string testList, std::string tar)
{
	double th_d=0.1*1e7;
	double th_v=-2.0*1e3;


	int totalsum=0,totalhit=0,subsum,subhit;
	int totalcough=0,totalreco=0,subreco;
	int totalfiller=0;
	FILE* f=fopen(testList.c_str(),"r");
	FILE* t=NULL;
	unsigned int i;
	char buf[1024];

	t=fopen(tar.c_str(),"w");
	while(fgets(buf,1024,f)!=NULL)
	{
		subreco=0;
		subsum=0;
		subhit=0;
		std::string fname=getFileNameWithOutExt(std::string(buf));
		std::string ol=labDir+"/"+fname+"."+labExt;
		std::string nl=recDir+"/"+fname+"."+labExt;
		std::vector< struct segment_ > vec;
		FILE *fp=fopen(ol.c_str(),"r");
		while(fgets(buf,1024,fp)!=NULL)
		{
			double u,v;
			char flag[32];
			sscanf(buf,"%lf\t%lf\t%s",&u,&v,flag);
			if(strcmp(flag,"cough")==0)
				vec.push_back(segment_(u,v));
		}
		fclose(fp);
		char *hashtb=new char[vec.size()];
		for(i=0;i<vec.size();i++)
			hashtb[i]=0;
		totalcough+=vec.size();
		fp=fopen(nl.c_str(),"r");
		while(fgets(buf,1024,fp)!=NULL)
		{
			double u,v;
			double lp;
			char flag[32];
			sscanf(buf,"%lf\t%lf\t%s%lf",&u,&v,flag,&lp);
			if(strncmp(flag,"cough",5)==0)
			{
				int ms=1;
				subsum++;
				for(i=0;i<vec.size();i++)
				{
					int hs=0;
					double m=(vec[i].u+vec[i].v)/2;
					if(decimalCmp(u,m,EPS)<=0&&decimalCmp(v,m,EPS)>=0)
					{
						hs=1;
					}
					if(hs)
					{
						if(hashtb[i]==0)
						{
							hashtb[i]=1;
							subreco++;
						}
						ms=0;
					}
				}
				if(!ms)
						subhit++;
			}
			else if(strncmp(flag,"filler",6)==0)
				totalfiller++;
		}
		fclose(fp);
		fprintf(t,"%s\t%d\t%d\t%d\t%d\n",fname.c_str(),subhit,subsum,subreco,vec.size());
		totalreco+=subreco;
		totalhit+=subhit;
		totalsum+=subsum;
		delete hashtb;
	}
	fprintf(t,"HIT/SUM\t%d\t%d\t%.1lf%%\n",totalhit,totalsum,totalhit*100.0/totalsum);
	fprintf(t,"RECO/FLAG\t%d\t%d\t%.1lf%%\n",totalreco,totalcough,totalreco*100.0/totalcough);
	fprintf(t,"FILLER\t%d\n",totalfiller);
	fclose(f);
	fclose(t);
}
int decimalCmp(double a,double b,double eps)
{
	if(a-b>eps)
		return 1;
	else if(b-a>eps)
		return -1;
	else
		return 0;
}
void writeCommonProto(std::string tar,std::string tarKind,int numState,int vecsize,int nummixes,std::string hmmName)
{
	int i,j,k;
	FILE *fp=fopen(tar.c_str(),"w");
	fprintf(fp,"~o <VecSize> %d <%s>\n",vecsize,tarKind.c_str());
	fprintf(fp,"~h \"%s\"\n",hmmName.c_str());
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
		mat[i][i]=0.4;
		mat[i][i+1]=0.4;
		mat[i][numState-1]+=0.2;
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
void writeCommonGram(std::string tar,std::string cHmm,std::string nHmm)
{
	FILE *fp=fopen(tar.c_str(),"w");
	fprintf(fp,"(< [%s] <%s> >)",cHmm.c_str(),nHmm.c_str());
	fclose(fp);
}
int calcuLabels(std::vector<std::string> &vec,std::vector<int> &vcount)
{
	int cnt=0;
	for(int i=0;i<vec.size();i++)
	{
		FILE *fp=fopen(vec[i].c_str(),"r");
		int tcnt=0;
		while(true)
		{
			char buf[32];
			double a,b;
			int ret=fscanf(fp,"%lf\t%lf\t%s",&a,&b,buf);
			if(ret==-1)
				break;
			trimStr(buf);
			if(strcmp(buf,"cough")==0)
				tcnt++;
		}
		vcount.push_back(tcnt);
		cnt+=tcnt;
		fclose(fp);
	}
	return cnt;
}
std::vector<std::string> readLabListByMfccList(std::vector<std::string> &mfccList,std::string labDir)
{
	std::vector<std::string> ret;
	for(int i=0;i<mfccList.size();i++)
	{
		std::string tar=getLabPathByMfccPath(mfccList[i],labDir);
		ret.push_back(tar);
	}
	return ret;
}
std::string getMfccPathByLabPath(std::string labPath,std::string mfccDir)
{
	std::string fname=getFileNameWithOutExt(labPath);
	return mfccDir+"/"+fname+"."+mfccExt;
}
std::string getLabPathByMfccPath(std::string mfccPath,std::string labDir)
{
	std::string fname=getFileNameWithOutExt(mfccPath);
	return labDir+"/"+fname+"."+labExt;
}

void divideDataList(std::vector<std::string> labelList,std::vector<std::string> dataList,std::string &trainListPath,std::string &testListPath,int nfold,int curfold)
{
	int i;
	std::vector<std::string> trainList;
	std::vector<std::string> testList;
	int per=labelList.size()/nfold;
	int st=curfold*per,ed=(curfold+1)*per;
	if(curfold+1==nfold)
		ed=labelList.size();
	for(i=0;i<labelList.size();i++)
	{
		if(i<st||i>=ed)
			trainList.push_back(dataList[i]);
		else
			testList.push_back(dataList[i]);
	}
	writeListFile(trainListPath,trainList);
	writeListFile(testListPath,testList);
	printf("%d\t%d\t%d\n",curfold,trainList.size(),testList.size());
}

int byte2int(unsigned char* bytes,int bn)
{
	int i;
	int ret=0;
	for(i=0;i<bn;i++)
	{
		ret=ret<<8;
		ret=ret|bytes[i];
	}
	return ret;
}
void int2byte(int inter,unsigned char* bytes,int bn)
{
	int i;
	for(i=0;i<bn;i++)
	{
		bytes[bn-i-1]=(unsigned char)(inter&0x000000ff);
		inter=inter>>8;
	}
}

int segFilter(std::string mfccPath,float aboveTh)
{
	static const int seglen=624;
	static const int seglen3=1872;
	int indpos=mfccPath.rfind("_");
	int dotpos=mfccPath.rfind(".");
	int idx,tidx,i;
	std::string ind=mfccPath.substr(indpos+1,dotpos-indpos-1);
	std::string tind;
	int pext=0,lext=0;
	int alen=0,acount;
	float segcen[seglen];
	float segcmp[seglen3];
	mfccReader mr;

	mr.readEnergy((char*)mfccPath.c_str(),segcen);
	alen+=seglen;
	memcpy(segcmp,segcen,seglen*sizeof(int));

	ss.clear();
	ss<<ind;
	ss>>idx;
	ss.clear();

	tidx=idx-1;
	ss<<tidx;
	ss>>tind;
	std::string prePath=mfccPath.substr(0,indpos+1)+tind+"."+mfccExt;
	struct stat buf;
	if(stat(prePath.c_str(),&buf)==0)
	{
		pext=1;
		mr.readEnergy((char*)prePath.c_str(),segcmp+alen);
		alen+=seglen;
	}

	tidx=idx+1;
	ss<<tidx;
	ss>>tind;
	std::string latPath=mfccPath.substr(0,indpos+1)+tind+"."+mfccExt;
	if(stat(latPath.c_str(),&buf)!=0)
	{
		lext=1;
		mr.readEnergy((char*)latPath.c_str(),segcmp+alen);
		alen+=seglen;
	}
	
	std::sort(segcmp,segcmp+alen);
	
	float thresh=segcmp[int(alen*0.9)];

	int tmp=0;
	for(i=0;i<seglen;i++)
	{
		if(segcen[i]>=thresh+aboveTh)
			tmp++;
		else
			tmp=0;
		if(tmp>=3)
			return 0;
	}
	return -1;
}

float byte2float(unsigned char* byte)
{
	unsigned char buf[4];
	int i;
	for(i=0;i<4;i++)
	{
		buf[3-i]=byte[i];
	}
	return *((float*)buf);
}

void filterList(std::string srcPath,std::string tarPath,float aboveTh)
{
	int i;
	std::vector<std::string> mfccVec=readListFile(srcPath);
	std::vector<std::string> tmpVec;
	for(i=0;i<mfccVec.size();i++)
	{
		if(segFilter(mfccVec[i],aboveTh)==0)
			tmpVec.push_back(mfccVec[i]);
	}
	printf("%d->%d\n",mfccVec.size(),tmpVec.size());

	writeListFile(tarPath,tmpVec);

}
