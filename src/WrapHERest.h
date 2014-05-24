#pragma once
extern "C" int mainHERest(int argc, char *argv[]);
class WrapHERest
{
public:
	void init(std::string pTrainList,
	std::string pTarDir,
	std::string pmmf,
	std::string pmmf2,
	std::string pLabelDir,
	std::string pHmmList);
	void run();
private:
	std::string trainList;
	std::string tarDir;
	std::string mmf;
	std::string mmf2;
	std::string labelDir;
	std::string hmmList;
};