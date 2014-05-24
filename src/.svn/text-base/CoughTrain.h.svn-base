#pragma once
#include <vector>
class CoughTrain
{
public:
	void init(std::string inWorkingDir,std::string inMfccList,std::string inLabelDir);
	void run(int repTimes);
	/*std::string getGlobalInf();*/
	std::string getOutputDir();
	std::string getScoredLabelDir();
	std::string getMfccList();
private:
	void writeCoughProtoFile(std::string tar,std::string tarKind,int numState,int vecsize,int nummixes);
	void updateLabels(std::string tarDir,std::string srcDir,std::string scoreDir,std::string elist);
private:
	std::string workingDir;
	std::string mfccList;
	std::string tmpDir;
	std::string labelDir;
	std::string modelDir;
	std::string scoredLabelDir;
	std::string outputDir;
	std::string labeledList;
	std::vector<std::string> modellist;
	std::vector<std::string> sublabList;
};