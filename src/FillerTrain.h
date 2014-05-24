#pragma once
extern "C"
{
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
}
struct train_info
{
	std::string *ptr_mfccList;
	std::string *ptr_fillerLabelDir;
	std::string *ptr_curdir;
	int *ptr_filler_counter;
	int *ptr_filler_total;
};
class FillerTrain
{
public:
	void init(std::string inWorkingDir,std::string inMfccList);
	void run(int repTimes,double var);
	std::string getOutputDir();
	std::string getFillerLabelDir();
	static void* mul_thread_train_filler(void *arg);
	void writeFillerProtoFile(std::string tar,std::string tarKind,int numState,int vecsize,int nummixes);
	void updateFillerHmm(std::string src,std::string tar,int idx,double var,int vecsize);
private:
	std::string workingDir;
	std::string mfccList;
	std::string tmpDir;
	std::string fillerLabelDir;
	std::string modelDir;
	std::string outputDir;
	int filler_counter;
	int filler_total;
	const static int thread_num=4;
	static pthread_mutex_t mutex_count;
	static pthread_mutex_t mutex_data;
};
