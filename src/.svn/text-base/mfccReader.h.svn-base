#include <stdio.h>


struct mfcc_head
{
	int nSamples;
	int sampPeriod;
	int sampSize;
	int parmKind;
};
typedef struct mfcc_head mfccHead;

class mfccReader
{
public:
	mfccReader(void);
	void subFile(char* srcPath,char* tarPath,int fromNs,int toNs);
	int readEnergy(char* srcPath,float* e);
	~mfccReader(void);
public:
	const static int headSize=12;
};
