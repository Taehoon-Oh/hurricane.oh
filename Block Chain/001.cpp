#define EE 1



#if EE

#include<stdio.h>
#define log(a,...)	printf("%10s[%03d]:	" a "\n",__FUNCTION__,__LINE__,##__VA_ARGS__)


#else

#endif


#define F(a,b,c)	for( register int a=b;a<=c;a++)

using namespace std;


#define MAXSERVER 5
#define IMAGESIZE 400000
#define MAXHASH 15000
#define MAXBLK MAXHASH
#define MAX_TABLE MAXHASH
#define MAX (MAXHASH*2+7)




class Block {
public:
	int cnt; 
	unsigned int  hash;
	unsigned int  prehash;
	short random;
	char tnum;
	char tid[16]; 
	short tant[16]; 

	Block* sib;
	Block* child;
};

Block tb[MAX];

//int gbc = 0; 
//
//Block* newBlock() {
//	Block* bp = &block[gbc++]; 
//	bp->hash = bp->prehash = 0; 
//	bp->random[0] = bp->random[1] = 0; 
//	bp->sib = bp->child = 0; 
//	return bp; 
//}

class Buf {
public:
	int hash;
	int start;
	int len; 
};


extern int calcHash(unsigned char buf[], int pos, int len);

int find(int key )
{
	int h = key % MAX; 
	int cnt = MAX;

	while (tb[h].hash != -1 && cnt--)
	{
		if (tb[h].hash == key)
		{
			return h;
		}
		h = (h + 1) % MAX;
	}
	return 0;
}

int add(int key)
{
	int h = key % MAX;

	while (tb[h].hash != -1)
	{
		if (tb[h].hash == key)
		{
			return h; 
		}

		h = (h + 1) % MAX;
	}
	return h;
}



unsigned char blockchainimage[MAXSERVER][IMAGESIZE]; 


void syncBlockChain(int S, unsigned char img[MAXSERVER][IMAGESIZE]) {
	F(i, 0, MAX-1) {
		tb[i].hash = -1; 
		tb[i].cnt = 0;
	}


	F(i, 0, S - 1) {
		int len = (img[i][0] << 24) + (img[i][1] << 16 )+ (img[i][2] << 8 ) + img[i][3]; 
		unsigned char *cur = &img[i][4]; 

		F(j, 0, len - 1) {			
			int hash; 
			int blklen = 4 + 2 + 1 + 3 * cur[6];
			hash= calcHash(cur, 0, blklen); 
			int h = add(hash); 
			tb[h].hash = hash; 
			tb[h].prehash = cur[0] << 24 + cur[1] << 16 + cur[2] << 8 + cur[3]; 
			tb[h].cnt++; 

			tb[h].tnum = cur[6];
			int tnum = cur[6]; 
			//log("block id = %d, hash= %08x , preblock = %08x, tnum = %d, ", h, hash, tb[h].prehash, tb[h].tnum);

			while (tnum--) {
				tb[h].tid[tnum] = cur[7]; 
				tb[h].tant[tnum] =( cur[8]*256) + cur[9];
				log("n= %d , id = %d , amount=%d ", tnum, tb[h].tid[tnum], tb[h].tant[tnum]);
			}
			
			j += blklen; 
			cur += blklen;
		}
	}
}

int calcAmount(int hash, int exchangeid) {
	return 0;
}



/*
void dfs(){
do somethings 

for ()
 dfs
}

void dosomethings()

*/