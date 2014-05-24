#pragma once
#include "glb.h"
#include "WrapHHEd.h"
void WrapHHEd::init(std::string inSrc,std::string inTarDir,std::string inEditF,std::string inHmmList)
{
	src=inSrc;
	tarDir=inTarDir;
	editF=inEditF;
	hmmList=inHmmList;
}
void WrapHHEd::run()
{
	char *(argv[])={"HHEd","-H",(char*)src.c_str(),"-M",(char*)tarDir.c_str(),(char*)editF.c_str(),(char*)hmmList.c_str()};
	int argc=sizeof(argv)/sizeof(char*);
	mainHHEd(argc,argv);
}