#pragma once
extern "C" int mainHVite(int argc, char *argv[]);
class WrapHVite
{
private:
	std::string hmmsdef;
	std::string hmmsdef2;
	std::string reco;
	std::string net;
	std::string dict;
	std::string hmmlist;
	std::string input;
public:
	void init(std::string inHmmsdef,std::string inHmmsdef2,
	std::string inReco,
	std::string inNet,
	std::string inDict,
	std::string inHmmlist,
	std::string inInput);
	void run();
	void run1();
	void run2();
	void run3();
	void runS2();
};