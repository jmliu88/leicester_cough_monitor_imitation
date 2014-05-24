#include "trainModel.h"
#include "src/mfccReader.h"
#include "src/Util.h"
#include "src/WrapHInit.h"
#include "src/WrapHRest.h"
#include <vector>
#include "string.h"
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
trainModel::trainModel(void)
{

}

void trainModel::init(std::string workingDir)
{
	modelDir=workingDir+"/s2model";
	outputDir=workingDir+"/s2trainEvt";
	coughModelPath=modelDir+"/"+s2CoughModel+"."+hmmExt;
	noncoughModelPath=modelDir+"/"+s2NoncoughModel+"."+hmmExt;
	coughListPath=outputDir+"/coughList.txt";
	noncoughListPath=outputDir+"/noncoughList.txt";
	createEmptyDir(modelDir);
	createEmptyDir(outputDir);
}

std::string trainModel::getModelDir()
{
	return modelDir;
}
void trainModel::run()
{
	writeCommonProto(coughModelPath,gTarKind,20,39,1,s2CoughModel);
	writeCommonProto(noncoughModelPath,gTarKind,20,39,1,s2NoncoughModel);

	WrapHInit wi;
	wi.init(coughListPath,modelDir,coughModelPath,"","",s2CoughModel);
	wi.runS2();
	WrapHRest wr;
	wr.init(coughListPath,modelDir,coughModelPath,"","",s2CoughModel,"");
	wr.runS2();

	wi.init(noncoughListPath,modelDir,noncoughModelPath,"","",s2NoncoughModel);
	wi.runS2();
	wr.init(noncoughListPath,modelDir,noncoughModelPath,"","",s2NoncoughModel,"");
	wr.runS2();
	
}

void trainModel::prepareData(std::string recDir,std::string labDir,std::string eventList)
{
	int coughCount=0;
	int noncoughCount=0;
	std::vector<std::string> coughlist;
	std::vector<std::string> noncoughlist;




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

			/*ignore too short segment*/
			if(v-u<minSegNs)
				continue;

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
				if(iscough)
				{	
					sprintf(subFilePath,"%s/cough%d.%s",outputDir.c_str(),coughCount++,mfccExt.c_str());
					coughlist.push_back(std::string(subFilePath));
				}
				else
				{
					sprintf(subFilePath,"%s/noncough%d.%s",outputDir.c_str(),noncoughCount++,mfccExt.c_str());
					noncoughlist.push_back(std::string(subFilePath));
				}
				mr.subFile((char*)mfccPath.c_str(),subFilePath,u,v);
				
			}
		}
		fclose(fp);
	}
	fclose(f);

	writeListFile(coughListPath,coughlist);
	writeListFile(noncoughListPath,noncoughlist);
}

trainModel::~trainModel(void)
{

}
