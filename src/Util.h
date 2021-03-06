#pragma once
#include "vector"
#include "wavformat.h"

static int ns[]={10,15,25};
static int coughMix=12;
static int msPerFrame=26;
static int fillerStateNum=5;
static int fillerMix=3;
static int minSegNs=2900000;
static std::string labExt="lab";
static std::string hmmExt="hmm";
static std::string mfccExt="mfcc";
static std::string tgExt="TextGrid";
static std::string gTarKind="MFCC_0_Z_D_A";
static std::string s2CoughModel="cough";
static std::string s2NoncoughModel="noncough";
static std::string confName="myconf.txt";


//static std::string gTarKind="MFCC_0_D_A";
static double EPS=1e-6;

void trimStr(char *buf);
void travelDir(std::string rootDir,std::string suffix,std::vector<std::string> *vec);
void createEmptyDir(std::string dir);
std::string getFileNameWithExt(std::string src);
std::string getFileNameWithOutExt(std::string src);
std::string convertPathFormat(std::string src);
void tgToLabel(std::string src,std::string tarDir);
std::vector<std::string> readListFile(std::string tar);
int decimalCmp(double a,double b,double eps);
void writeListFile(std::string tar,std::vector<std::string> &vec);
void combineListFile(std::string src1,std::string src2);
void writeConfFile(std::string tar,std::string tarKind,double winSize,double tarRate);
void writeHHEdFile(int singleState,std::string cmdTar,std::string hmmlistTar,int ns,int mixnum,std::vector<std::string> mName);
void writeHmmListAndDefFile(std::string tarList,std::string tarDef,std::vector<std::string> vecHmm,bool mmf);
void writeGram(std::string tar,std::vector<std::string> vec);
void writeGramBinary(std::string tar,std::vector<std::string> vec);
void writeDict(std::string tar,std::vector<std::string> vec);
void writeDict(std::string tar,std::string hmmName,int st,int ed);
void writeGram(std::string tar,std::string hmmName,int st,int ed);
void combineLabels(std::string cLabel,std::string fLabel,std::string trainlist,std::string emptylist,std::string tLabel);
void combineListFile(std::string tarPath,std::string src1,std::string src2);
void copyAudioLabelPair(std::string aSrcDir,std::string lSrcDir,std::string aTarDir,std::string lTarDir,std::string labFlag);
void evaluateResult(std::string recDir,std::string labDir,std::string testList, std::string tar);

void writeCommonProto(std::string tar,std::string tarKind,int numState,int vecsize,int nummixes,std::string hmmName);
void writeCommonGram(std::string tar,std::string cHmm,std::string nHmm);
int calcuLabels(std::vector<std::string> &vec,std::vector<int> &vcount);
std::vector<std::string> readLabListByMfccList(std::vector<std::string> &mfccList,std::string labDir);
std::string getMfccPathByLabPath(std::string labPath,std::string mfccDir);
std::string getLabPathByMfccPath(std::string mfccPath,std::string labDir);

void divideDataList(std::vector<std::string> labelList,std::vector<std::string> dataList,std::string &trainListPath,std::string &testListPath,int nfold,int curfold);
int byte2int(unsigned char* bytes,int bn);
void int2byte(int inter,unsigned char* bytes,int bn);
float byte2float(unsigned char* byte);

int segFilter(std::string mfccPath,float aboveTh);
void filterList(std::string srcPath,std::string tarPath,float aboveTh);
