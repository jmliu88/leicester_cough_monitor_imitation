#pragma once
#include "glb.h"
#include "WrapHParse.h"
void WrapHParse::init(std::string inGram, std::string inNet)
{
	gram=inGram;
	net=inNet;
}
void WrapHParse::run()
{
	char *(argv[])={"HParse",(char*)gram.c_str(),(char*)net.c_str()};
	int argc=sizeof(argv)/sizeof(char*);
	mainHParse(argc,argv);
}