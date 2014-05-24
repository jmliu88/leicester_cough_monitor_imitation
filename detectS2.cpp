#include "detectS2.h"
#include <vector>
#include "src/mfccReader.h"
#include "src/Util.h"
#include "src/WrapHVite.h"
#include "string.h"
#include "src/WrapHParse.h"
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
detectS2::detectS2(void)
{

}

detectS2::~detectS2(void)
{

}

void detectS2::init(std::string imodelDir,std::string workingDir)
{
	modelDir=imodelDir;
	outputDir=workingDir+"/s2testEvt";
	s2recDir=workingDir+"/s2rec";
	coughModelPath=modelDir+"/"+s2CoughModel+"."+hmmExt;
	noncoughModelPath=modelDir+"/"+s2NoncoughModel+"."+hmmExt;
	eventListPath=outputDir+"/eventList.txt";
	shortEventListPath=outputDir+"/shortEvtList.txt";
	createEmptyDir(outputDir);
	createEmptyDir(s2recDir);
}
void detectS2::run()
{
	std::vector<std::string> hmmlist;
	std::string hmmlistPath=modelDir+"/hmmlist.txt";
	std::string dictPath=modelDir+"/dict.txt";
	std::string gramPath=modelDir+"/gram.txt";
	std::string netPath=modelDir+"/net.txt";
	hmmlist.push_back(s2CoughModel);
	hmmlist.push_back(s2NoncoughModel);
	writeListFile(hmmlistPath,hmmlist);
	writeDict(dictPath,hmmlist);
	writeGramBinary(gramPath,hmmlist);

	WrapHParse wp;
	wp.init(gramPath,netPath);
	wp.run();

	WrapHVite wv;
	wv.init(coughModelPath,noncoughModelPath,s2recDir,netPath,dictPath,hmmlistPath,eventListPath);
	wv.runS2();
}

void detectS2::prepareData(std::string recDir,std::string labDir,std::string eventList)
{
	std::vector<std::string> evtlist;
	std::vector<std::string> shortEvtlist;



	FILE* f=fopen(eventList.c_str(),"r");
	unsigned int i;
	char buf[1024];

	while(fgets(buf,1024,f)!=NULL)
	{
		trimStr(buf);
		std::string mfccPath=std::string(buf);
		std::string fname=getFileNameWithOutExt(mfccPath);
		std::string ol=labDir+"/"+fname+"."+labExt;
		std::string nl=recDir+"/"+fname+"."+labExt;
		std::vector<segment_> vec;
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

		mfccReader mr;
		fp=fopen(nl.c_str(),"r");
		while(fgets(buf,1024,fp)!=NULL)
		{
			double u,v;
			double lp;
			char flag[32];
			sscanf(buf,"%lf\t%lf\t%s%lf",&u,&v,flag,&lp);

			
			if(strncmp(flag,"cough",5)==0)
			{
				int iscough=0;
				for(i=0;i<vec.size();i++)
				{
					double m=(vec[i].u+vec[i].v)/2;
					if(decimalCmp(u,m,EPS)<=0&&decimalCmp(v,m,EPS)>=0)
					{
						iscough=1;
						break;
					}
				}
				char subFilePath[256];

				/*handle too short segment*/
				if(v-u<minSegNs)
				{
					if(iscough)
						sprintf(subFilePath,"%s/1_%s_%.0lf_%.0lf.%s",s2recDir.c_str(),fname.c_str(),u,v,labExt.c_str());
					else
						sprintf(subFilePath,"%s/0_%s_%.0lf_%.0lf.%s",s2recDir.c_str(),fname.c_str(),u,v,labExt.c_str());
					FILE* fptr=fopen(subFilePath,"w");
					fprintf(fptr,"0\t0\tcough\t0\n");
					fclose(fptr);
					shortEvtlist.push_back(std::string(subFilePath));
				}
				else
				{
					if(iscough)
						sprintf(subFilePath,"%s/1_%s_%.0lf_%.0lf.%s",outputDir.c_str(),fname.c_str(),u,v,mfccExt.c_str());
					else
						sprintf(subFilePath,"%s/0_%s_%.0lf_%.0lf.%s",outputDir.c_str(),fname.c_str(),u,v,mfccExt.c_str());
					evtlist.push_back(std::string(subFilePath));
					mr.subFile((char*)mfccPath.c_str(),subFilePath,u,v);
				}
				
			}
		}
		fclose(fp);
	}
	fclose(f);

	writeListFile(eventListPath,evtlist);
	writeListFile(shortEventListPath,shortEvtlist);
}
