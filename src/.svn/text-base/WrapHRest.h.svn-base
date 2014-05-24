#pragma once
extern "C" int mainHRest(int argc, char *argv[],char *inScorePath);
class WrapHRest
{
	public:
	void init(std::string inTrainList,std::string inTarDir,std::string inSrc,std::string inLabel,std::string inLabelDir,std::string inHmmName,std::string inScorePath);
	void run();
	void runS2();
private:
	std::string trainList;
	std::string tarDir;
	std::string src;
	std::string label;
	std::string labelDir;
	std::string hmmName;
	std::string scorePath;
};