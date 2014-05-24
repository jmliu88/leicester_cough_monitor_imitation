#pragma once
#include "glb.h"
#include "WrapHInit.h"
void WrapHInit::init(std::string inTrainList,std::string inTarDir,std::string inSrc,std::string inLabel,std::string inLabelDir,std::string inHmmName)
{
	trainList=inTrainList;
	tarDir=inTarDir;
	src=inSrc;
	label=inLabel;
	labelDir=inLabelDir;
	hmmName=inHmmName;
}
void WrapHInit::run()
{
	char *(argv[])={"HInit -m 1 ","-S",(char*)trainList.c_str(),"-M",(char*)tarDir.c_str(),"-H",(char*)src.c_str(),"-l",(char*)label.c_str(),"-L",(char*)labelDir.c_str(),(char*)hmmName.c_str()};
	int argc=sizeof(argv)/sizeof(char*);
	mainHInit(argc,argv);
}

//loading complete file
void WrapHInit::runS2()
{
	char *(argv[])={"HInit -m 1 ","-S",(char*)trainList.c_str(),"-M",(char*)tarDir.c_str(),"-H",(char*)src.c_str(),(char*)hmmName.c_str()};
	int argc=sizeof(argv)/sizeof(char*);
	mainHInit(argc,argv);
}