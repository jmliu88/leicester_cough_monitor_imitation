#pragma once
#include "glb.h"
#include "Recognition.h"
#include "Util.h"
#include "htks.h"
void Recognition::init(std::string inWorkingDir,std::string inCoughMmf,std::string inFillerMmf,std::string inHmmList)
{
	workingDir=inWorkingDir;
	coughMmf=inCoughMmf;
	fillerMmf=inFillerMmf;
	hmmList=inHmmList;
	tmpDir=workingDir+"/"+"tmp";
	outputDir=workingDir+"/"+"s1rec";
}
std::string Recognition::getOutputDir()
{
	return outputDir;
}
void Recognition::run(std::string mfccList)
{
	createEmptyDir(workingDir);
	createEmptyDir(tmpDir);
	createEmptyDir(outputDir);

	std::string tdict=tmpDir+"/dict.txt";
	std::string tgram=tmpDir+"/gram.txt";
	std::string tnet=tmpDir+"/net.txt";

	std::vector<std::string> vecM=readListFile(hmmList);
	//make dict gram net mmf
	WrapHParse wp;
	writeDict(tdict,vecM);
	writeGram(tgram,vecM);
	wp.init(tgram,tnet);
	wp.run();

	WrapHVite wv;
	wv.init(coughMmf,fillerMmf,outputDir,tnet,tdict,hmmList,mfccList);
	wv.run2();
}
