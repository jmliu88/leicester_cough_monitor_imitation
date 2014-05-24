#pragma once
#include <map>
#include<vector>
class cut_node
{
public:
	int interval;
	double count;
	cut_node();
	cut_node(int interval,int count)
	{
		this->interval=interval;
		this->count=count;
	}
};
class PreProcess
{
public:
	void init(std::string inWorkingDir,std::string inAudioDir,std::string inLabelDir);
	void run(int frame_th,int low_th,bool tg,bool demp3);
	std::string getMfccList();
	std::string getSplitedLabelDir();
	std::string getLabeledList();
	std::string getEmptyList();
	void writeLabeledList(std::string tar,std::string srcComplete,std::string srcEmpty);
	std::vector<int> splitLabel(std::string src,std::string tarDir,std::string infDir);
	void makeupTg(std::string audioDir,std::string tgDir,std::string audioExt,std::string tgExt);

private:
	std::string workingDir;
	std::string audioDir;
	std::string mfccDir;
	std::string mfccList;
	std::string tmpDir;
	std::string labelDir;
	std::string splitedLabelDir;
	std::string emptyList;
	std::string labeledList;
	std::map<std::string,cut_node> cut_map;
};

