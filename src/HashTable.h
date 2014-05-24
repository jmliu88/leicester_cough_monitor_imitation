#pragma once
#include "string.h"
class HashTable
{
public:
	void insert(char* str);
	int check(char* str);
	HashTable();
	~HashTable();
	void reset();
	void init();
private:
	unsigned int ELFHash(const char * str);
private:
	const static int MOD=9997;
	typedef struct tableHead
	{
		char str[64];
		 tableHead *next;
		 tableHead()
		 {
		 }
		 tableHead(char *ss)
		 {
			 next=NULL;
			 strcpy(str,ss);
		 }
	}thead;
	thead tb[MOD];
};
