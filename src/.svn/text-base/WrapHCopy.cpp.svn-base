#pragma once
#include "glb.h"
#include "WrapHCopy.h"
void WrapHCopy::init(std::string inConf,std::string inSrc,std::string inTar)
{
	conf=inConf;
	src=inSrc;
	tar=inTar;
}
void WrapHCopy::run()
{
	char *(argv[])={"HCopy","-C",(char*)conf.c_str(),(char*)src.c_str(),(char*)tar.c_str()};
	int argc=sizeof(argv)/sizeof(char*);
	mainHCopy(argc,argv);
}