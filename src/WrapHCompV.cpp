#pragma once
#include "glb.h"
#include "WrapHCompV.h"
void WrapHCompV::init(std::string inTrainList,std::string inTarDir,std::string inSrc,std::string inHmmName)
{
	trainList=inTrainList;
	tarDir=inTarDir;
	src=inSrc;
	hmmName=inHmmName;
}
void WrapHCompV::run()
{
	char *(argv[])={"HCompV","-S",(char*)trainList.c_str(),"-M",(char*)tarDir.c_str(),"-H",(char*)src.c_str(),(char*)hmmName.c_str()};
	int argc=sizeof(argv)/sizeof(char*);
	mainHCompV(argc,argv);
}