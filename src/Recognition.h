#pragma once
class Recognition
{
public:
	void init(std::string inWorkingDir,std::string inCoughMmf,std::string inFillerMmf,std::string inHmmList);
	void run(std::string mfccList);
	std::string getOutputDir();
public:
	std::string workingDir;
	std::string outputDir;
	std::string tmpDir;
	std::string coughMmf;
	std::string fillerMmf;
	std::string hmmList;
};
