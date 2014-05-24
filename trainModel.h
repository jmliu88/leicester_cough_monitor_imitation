#pragma once
#include <string>
class trainModel
{
public:
	trainModel(void);
	void init(std::string workingDir);
	void run();
	void prepareData(std::string recDir,std::string labDir,std::string eventList);
	~trainModel(void);
	std::string getModelDir();
public:
	std::string modelDir;
	std::string coughModelPath;
	std::string noncoughModelPath;
	std::string outputDir;
	std::string coughListPath;
	std::string noncoughListPath;
};
