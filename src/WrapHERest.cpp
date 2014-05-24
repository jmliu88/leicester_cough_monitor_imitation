#pragma once
#include "glb.h"
#include "WrapHERest.h"
void WrapHERest::init(	std::string pTrainList,
	std::string pTarDir,
	std::string pmmf,
	std::string pmmf2,
	std::string pLabelDir,
	std::string pHmmList)
{
	trainList=pTrainList;
	tarDir=pTarDir;
	mmf=pmmf;
	mmf2=pmmf2;
	labelDir=pLabelDir;
	hmmList=pHmmList;
}
void WrapHERest::run()
{
	char *(argv[])={"HERest -m 1 ","-S",(char*)trainList.c_str(),"-x","hmm","-M",(char*)tarDir.c_str(),"-H",(char*)mmf.c_str(),"-H",(char*)mmf2.c_str(),"-L",(char*)labelDir.c_str(),(char*)hmmList.c_str()};
	int argc=sizeof(argv)/sizeof(char*);
	mainHERest(argc,argv);
}
