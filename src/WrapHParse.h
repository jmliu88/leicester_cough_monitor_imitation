#pragma once
extern "C" int mainHParse(int argc, char *argv[]);
class WrapHParse
{
private:
	std::string gram;
	std::string net;
public:
	void init(std::string inGram,std::string inNet);
	void run();
};