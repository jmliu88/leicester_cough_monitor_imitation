#pragma once
#include "glb.h"
#include "EmbeddedTrain.h"
#include "Util.h"
#include "htks.h"
#include <vector>
std::string EmbeddedTrain::getHmmList()
{
	return hmmList;
}
std::string EmbeddedTrain::getCMMF()
{
	return mmf;
}
std::string EmbeddedTrain::getFMMF()
{
	return mmf2;
}
void EmbeddedTrain::init(std::string pWorkingDir,
		std::string pCLabDir,
		std::string pFLabDir,
		std::string pCModelDir,
		std::string pFModelDir,
		std::string pLabeledList,
		std::string pEmptyList)
{
	workingDir=pWorkingDir;
	tmpDir=workingDir+"/"+"tmp";
	tarDir=workingDir+"/"+"eModel";
	cLabDir=pCLabDir;
	fLabDir=pFLabDir;
	labDir=workingDir+"/"+"elabs";
	cModelDir=pCModelDir;
	fModelDir=pFModelDir;
	protommf=tmpDir+"/"+"eMMf.mmf";
	protommf2=tmpDir+"/"+"eMMf2.mmf";
	mmf=tarDir+"/"+"eMMf.mmf";
	hmmList=tarDir+"/"+"eHmmlist.txt";
	mmf2=tarDir+"/"+"eMMf2.mmf";
	hmmList2=tmpDir+"/"+"eHmmlist2.txt";
	labledList=pLabeledList;
	emptyList=pEmptyList;
	hmmhed=tmpDir+"/"+"hmmlist.hed";
	cmdhed=tmpDir+"/"+"cmd.hed";
	mfccList=workingDir+"/"+"eTrainlist.txt";
}
void EmbeddedTrain::run(int it1)
{
	int i;
	createEmptyDir(workingDir);
	createEmptyDir(labDir);
	createEmptyDir(tarDir);
	createEmptyDir(tmpDir);

	combineLabels(cLabDir,fLabDir,labledList,emptyList,labDir);
	combineListFile(mfccList,labledList,emptyList);

	std::vector<std::string> vec;
	travelDir(cModelDir,"hmm",&vec);
	writeHmmListAndDefFile(hmmList,protommf,vec,true);

	vec.clear();
	travelDir(fModelDir,"hmm",&vec);
	writeHmmListAndDefFile(hmmList2,protommf2,vec,true);
	
	combineListFile(hmmList,hmmList2);
	
	WrapHERest we;
	we.init(mfccList,tarDir,protommf,protommf2,labDir,hmmList);
	we.run();
	we.init(mfccList,tarDir,mmf,mmf2,labDir,hmmList);
	for(i=0;i<it1;i++)
		we.run();

	//hhed hmm
	//writeHHEdFile(cmdhed,hmmhed,fillerStateNum,fillerMix,vec);
	WrapHHEd wd;
	//std::string hmmName=tarDir+"/"+getFileNameWithExt(mmf2);
	//wd.init(hmmName,tarDir,cmdhed,hmmhed);
	//wd.run();
	for(i=0;i<vec.size();i++)
		vec[i]=getFileNameWithOutExt(vec[i]);
	for(i=2;i<fillerStateNum;i++)
	{
		writeHHEdFile(1,cmdhed,hmmhed,i,fillerMix,vec);
		std::string hmmName=tarDir+"/"+getFileNameWithExt(mmf2);
		wd.init(hmmName,tarDir,cmdhed,hmmhed);
		wd.run();

		we.run();
	}
}
