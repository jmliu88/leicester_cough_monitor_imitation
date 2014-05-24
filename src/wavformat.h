#ifndef WAVFORMAT_H
#define WAVFORMAT_H
#include <string>
#define BUF_MAX 1024*50
struct RIFF_HEADER
{
	unsigned char		szRiffID[4];  // 'R','I','F','F'
	unsigned int		dwRiffSize;
	unsigned char		szRiffFormat[4]; // 'W','A','V','E'
};

struct WAVE_FORMAT
{
	unsigned short		wFormatTag;
	unsigned short		wChannels;
	unsigned int		dwSamplesPerSec;
	unsigned int		dwAvgBytesPerSec;
	unsigned short		wBlockAlign;
	unsigned short		wBitsPerSample;
	//unsigned short		pack;		//∏Ωº”–≈œ¢
};
struct FMT_BLOCK
{
	unsigned char		szFmtID[4]; // 'f','m','t',' '
	unsigned int		dwFmtSize;
	struct	WAVE_FORMAT wavFormat;
};

struct FACT_BLOCK
{
	unsigned char		szFactID[4]; // 'f','a','c','t'
	unsigned int		dwFactSize;
};

struct DATA_BLOCK
{
	unsigned char		szDataID[4]; // 'd','a','t','a'
	unsigned int		dwDataSize;
};

struct WavHeader
{
	struct RIFF_HEADER riff;
	struct FMT_BLOCK fmt;
	//struct FACT_BLOCK fact;
	struct DATA_BLOCK data;
};
typedef struct WavHeader WavHeader;

struct reSampleHeader
{
	unsigned short		wChannels;
	unsigned int		dwSamplesPerSec;
	unsigned short		wBitsPerSample;
	double leftRatio;
	double rightRatio;
};
class wavFormat
{
public:
	wavFormat();
	~wavFormat();
	int audioOpen(std::string filePath);
	int reSample(std::string outPath,reSampleHeader* rsh);
	int subAudio(std::string outPath,int startInSec,int durationInSec);
	double totalDurationInSec();
	void audioClose();
private:
	unsigned int bufToUInt(char *buf,int bsize,int littleEndian);
public:
	WavHeader header;
	FILE *inFile;
	int PCMPos;
	int nSampleWidth;
	int nFrameRate;
	int nFrameNum;
	int nChannel;
};

#endif // WAVFORMAT_H
