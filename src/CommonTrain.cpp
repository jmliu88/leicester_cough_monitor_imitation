#include "glb.h"
#include "CommonTrain.h"
#include "Util.h"
#include "htks.h"
void CommonTrain::init(std::string inWorkingDir,
	std::string inLabelDir,
	std::string inTrainList)
{
	workingDir=inWorkingDir;
	labelDir=inLabelDir;
	trainList=inTrainList;
}
void CommonTrain::run(std::string modelPath,std::string labelFlag,std::string hmmName)
{
	createEmptyDir(workingDir);

	modelPath=convertPathFormat(modelPath);
	int idx=modelPath.rfind("/")+1;
	//std::string mmfName=modelPath.substr(idx,modelPath.length()-idx);
	std::string modelDir=modelPath.substr(0,idx-1);
	WrapHInit wi;
	wi.init(trainList,modelDir,modelPath,labelFlag,labelDir,hmmName);
	wi.run();

	WrapHRest wr;
	wr.init(trainList,modelDir,modelPath,labelFlag,labelDir,hmmName,"");
	wr.run();
}