#define EE 1

#if EE
#include <stdio.h>
#define Log(a,...) printf("%10s[%03d]: ",a,"\n",__FUNCTION__,__LINE__,##__VA_ARGS_)
#else
#define Log(a,...)
#endif 

#define Max(a,b) (((a)>(b))?(a):(b))
#define Min(a,b) (((a)<(b))?(a):(b))

#define F(a,b,c)	for(register int a =b; a<=c;a++)







void init(int N)
{
}

void follow(int uID1, int uID2, int timestamp)
{
}

void makePost(int uID, int pID, int timestamp)
{
}

void like(int pID, int timestamp)
{
}

void getFeed(int uID, int timestamp, int pIDList[])
{
}


