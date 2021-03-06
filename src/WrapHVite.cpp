#pragma once
#include "glb.h"
#include "WrapHVite.h"
void WrapHVite::init(std::string inHmmsdef,std::string inHmmsdef2,
	std::string inReco,
	std::string inNet,
	std::string inDict,
	std::string inHmmlist,
	std::string inInput)
{
	hmmsdef=inHmmsdef;
	hmmsdef2=inHmmsdef2;
	reco=inReco;
	net=inNet;
	dict=inDict;
	hmmlist=inHmmlist;
	input=inInput;
}
void WrapHVite::run()
{
	//char *(argv[])={"HVite","-S",input,"-H",hmmsdef,"-i",reco,"-w",net,dict,hmmlist};
	char *(argv[])={"HVite","-S",(char*)input.c_str(),"-d",(char*)hmmsdef.c_str(),"-x","hmm","-y","lab","-w",(char*)net.c_str(),"-l",(char*)reco.c_str(),(char*)dict.c_str(),(char*)hmmlist.c_str()};
	int argc=sizeof(argv)/sizeof(char*);
	mainHVite(argc,argv);
}
void WrapHVite::run1()
{
	char *(argv[])={"HVite","-S",(char*)input.c_str(),"-H",(char*)hmmsdef.c_str(),"-x","hmm","-y","lab","-w",(char*)net.c_str(),"-l",(char*)reco.c_str(),(char*)dict.c_str(),(char*)hmmlist.c_str()};
	int argc=sizeof(argv)/sizeof(char*);
	mainHVite(argc,argv);
}
void WrapHVite::run2()
{
	//char *(argv[])={"HVite","-S",(char*)input.c_str(),"-H",(char*)hmmsdef.c_str(),"-H",(char*)hmmsdef2.c_str(),"-x","hmm","-y","lab","-w",(char*)net.c_str(),"-l",(char*)reco.c_str(),(char*)dict.c_str(),(char*)hmmlist.c_str()};
	char *(argv[])={"HVite","-S",(char*)input.c_str(),"-H",(char*)hmmsdef.c_str(),"-H",(char*)hmmsdef2.c_str(),"-x","hmm","-y","lab","-w",(char*)net.c_str(),"-l",(char*)reco.c_str(),(char*)dict.c_str(),(char*)hmmlist.c_str()};
	int argc=sizeof(argv)/sizeof(char*);
	mainHVite(argc,argv);
}
void WrapHVite::run3()
{
	char *(argv[])={"HVite","-n","1","2","-S",(char*)input.c_str(),"-H",(char*)hmmsdef.c_str(),"-H",(char*)hmmsdef2.c_str(),"-x","hmm","-y","lab","-w",(char*)net.c_str(),"-l",(char*)reco.c_str(),(char*)dict.c_str(),(char*)hmmlist.c_str()};
	int argc=sizeof(argv)/sizeof(char*);
	mainHVite(argc,argv);
}
void WrapHVite::runS2()
{
	char *(argv[])={"HVite","-S",(char*)input.c_str(),"-H",(char*)hmmsdef.c_str(),"-H",(char*)hmmsdef2.c_str(),"-x","hmm","-y","lab","-w",(char*)net.c_str(),"-l",(char*)reco.c_str(),(char*)dict.c_str(),(char*)hmmlist.c_str()};
	int argc=sizeof(argv)/sizeof(char*);
	mainHVite(argc,argv);
}
