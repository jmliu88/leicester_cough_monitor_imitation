#include "mfccReader.h"
#include "Util.h"
#include "string.h"


mfccReader::mfccReader(void)
{

}
mfccReader::~mfccReader(void)
{

}
void mfccReader::subFile(char* srcPath,char* tarPath,int fromNs,int toNs)
{
	const int bufSize=1024;
	unsigned char buf[bufSize];
	FILE* fp=fopen(srcPath,"rb");
	mfccHead head;
	fread(buf,1,headSize,fp);
	head.nSamples=byte2int(buf,4);
	head.sampPeriod=byte2int(buf+4,4);
	head.sampSize=byte2int(buf+8,2);
	head.parmKind=byte2int(buf+10,2);
	
	int sampPerIO=bufSize/head.sampSize;
	int ss=fromNs/head.sampPeriod;
	int es=toNs/head.sampPeriod;
	fseek(fp,ss*head.sampSize,SEEK_CUR);
	int ct=(es-ss)/sampPerIO;
	int res=(es-ss)%sampPerIO;
	int i;
	mfccHead newHead;
	memcpy(&newHead,&head,sizeof(head));
	newHead.nSamples=es-ss;
	
	FILE* fw=fopen(tarPath,"wb");
	int2byte(newHead.nSamples,buf,4);
	fwrite(buf,1,4,fw);
	int2byte(newHead.sampPeriod,buf,4);
	fwrite(buf,1,4,fw);
	int2byte(newHead.sampSize,buf,2);
	fwrite(buf,1,2,fw);
	int2byte(newHead.parmKind,buf,2);
	fwrite(buf,1,2,fw);

	for(i=0;i<ct;i++)
	{
		fread(buf,head.sampSize,sampPerIO,fp);
		fwrite(buf,head.sampSize,sampPerIO,fw);
	}
	if(res>0)
	{
		fread(buf,head.sampSize,res,fp);
		fwrite(buf,head.sampSize,res,fw);
	}

	fclose(fw);
	fclose(fp);
}
int mfccReader::readEnergy(char* srcPath,float* e)
{
	const int bufSize=1024;
	unsigned char buf[bufSize];
	int i,j,scount=0;
	FILE* fp=fopen(srcPath,"rb");
	mfccHead head;
	fread(buf,1,headSize,fp);
	head.nSamples=byte2int(buf,4);
	head.sampPeriod=byte2int(buf+4,4);
	head.sampSize=byte2int(buf+8,2);
	head.parmKind=byte2int(buf+10,2);
	
	int sampPerIO=bufSize/head.sampSize;
	int ct=head.nSamples/sampPerIO;
	int res=head.nSamples%sampPerIO;

	for(i=0;i<ct;i++)
	{
		fread(buf,head.sampSize,sampPerIO,fp);
		for(j=0;j<sampPerIO;j++)
		{
			e[scount]=byte2float(buf+j*head.sampSize+48);
			scount++;
		}
	}
	if(res>0)
	{
		fread(buf,head.sampSize,res,fp);
		for(j=0;j<res;j++)
		{
			e[scount]=byte2float(buf+j*head.sampSize+48);
			scount++;
		}
	}
	fclose(fp);
	return scount;
}
