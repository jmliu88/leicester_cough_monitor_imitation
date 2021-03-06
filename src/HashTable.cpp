#pragma once
#include "glb.h"
#include "HashTable.h"
unsigned int HashTable::ELFHash(const char * str)
{
	  unsigned int hash = 0;
	  unsigned int x = 0;

	  while (*str) {
				  hash = (hash << 4) + (*str++);

				  if ((x = hash & 0xF0000000L ) != 0) {
							  hash ^= ( x >> 24);
							  hash &= ~x;
						}
			}

	  return hash&0x7FFFFFFF;
}
void HashTable::insert(char* str)
{
	unsigned int val=ELFHash(str)%MOD;
	thead *p=&tb[val];
	while(p->next!=NULL)
	{
		p=p->next;
	}
	p->next=new thead(str);
}
int HashTable::check(char* str)
{
	unsigned int val=ELFHash(str)%MOD;
	thead *p=tb[val].next;
	while(p!=NULL)
	{
		if(strcmp(p->str,str)==0)
			return 1;
		p=p->next;
	}
	return 0;
}
void HashTable::reset()
{
	int i;
	for(i=0;i<MOD;i++)
	{
		thead *p=tb[i].next,*pp;
		while(p!=NULL)
		{
			pp=p->next;
			delete p;
			p=pp;
		}
	}
}
void HashTable::init()
{
	int i;
	for(i=0;i<MOD;i++)
	{
		tb[i].next=NULL;
	}
}
HashTable::HashTable()
{
	init();
}
HashTable::~HashTable()
{
	reset();
}
