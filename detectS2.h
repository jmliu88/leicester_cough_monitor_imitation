#pragma once
#include <string>

class detectS2
{
public:
	detectS2(void);
	void init(std::string imodelDir,std::string workingDir);
	void run();
	void prepareData(std::string recDir,std::string labDir,std::string eventList);
	~detectS2(void);

public:
	std::string modelDir;
	std::string coughModelPath;
	std::string noncoughModelPath;
	std::string outputDir;
	std::string eventListPath;
	std::string s2recDir;
	std::string shortEventListPath;
};
