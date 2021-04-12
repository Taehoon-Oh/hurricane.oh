#define EE 0

#if EE
#include<stdio.h>
#define log(a,...)	 printf("%10s[%03d]: " a "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define log(a,...)
#endif 

template <typename T>
void swap(T &a, T &b) { T t = a; a = b; b = t; };
#define F(a,b,c)	for(register int a=b;a<=c;a++)

#define MAXL	5
#define MAXF	10
#define MAX  10001 
char flist[MAX][MAX]; 
bool fmap[MAX][MAX];


void init(int N)
{
	log("++++");
	F(i, 1, MAX - 1) {
		F(j, 1, MAX - 1) {
			fmap[i][j] = 0;
			flist[i][j] = 0;
		}
		flist[i][0] = 1, flist[i][1] = i; 
	}
	log("----\n");
}

void add(int id, int f, int ids[10])
{
	log("++++ id = %d , f =%d", id, f);
	F(i, 0, f-1) {
		fmap[id][ids[i]] = 1; 
		fmap[ids[i]][id] = 1;
		int fcnt = ++flist[id][0]; 
		flist[id][fcnt] = ids[i]; 

		fcnt = ++flist[ids[i]][0];
		flist[ids[i]][fcnt] = id; 
		
	}
	log("----\n");
}

void del(int id1, int id2)
{
	log("++++ id1=%d id2=%d ", id1,id2);
	fmap[id1][id2] = 0; 
	fmap[id2][id1] = 0;

	char fcnt = flist[id1][0]--;
	F(i, 2, fcnt) {
		if (flist[id1][i] == id2) {
			swap(flist[id1][i], flist[id1][fcnt]);
			break; 
		}
	}

	fcnt = flist[id2][0]--;
	F(i, 2, fcnt) {
		if (flist[id2][i] == id1) {
			swap(flist[id2][i], flist[id2][fcnt]);
			break; 
		}
	}
	log("----\n");
}




#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

int recommend(int id, int list[5])
{
	log("++++ id=%d ", id);
	int ret = 0;

	int fcnt = flist[id][0];
	log("fcnt=%d ", fcnt);
	char reco[10001] = { 0, };
	char rlist[10001] = { 0, }; 

	int max = 0; 

	F(i, 2, fcnt) {
		char fid = flist[id][i]; // 나의 친구 목록 
		log("fid=%d ", fid);

		int ffcnt = flist[fid][0]; 
		log("ffcnt=%d ", ffcnt);
		F(j, 2, ffcnt) {// 나의 친구가 아닌 것들에 대해서 하나씩 증가 시킨다. 

			int rfid = flist[fid][j]; 
			log("rfid=%d ", rfid);
			if (rfid == id ||fmap[id][rfid]) continue; 
			if (!reco[rfid]) {
				int rcnt = ++rlist[0];
				rlist[rcnt] = rfid; 
				log("rcnt = %d , rlist[ %d] =%d ", rcnt, rcnt, rlist[rcnt]); 
			}

			reco[rfid] ++;
			max = Max(max, reco[rfid]);
			log("recomment [%d] =%d ", rfid, reco[rfid]);


			/*int rcnt = rlist[0];

			F(k, 1, rcnt) {

			}*/
		}
	}

	int rcnt = rlist[0];
	 

	F(i, 1, Min( 5, rcnt -1) ) {
		
		F(j, i+1, rcnt) {
			int idi = rlist[i];
			int idj = rlist[j]; 
			if (reco[idi] == reco[idj] && idi > idj)swap(rlist[i], rlist[j]);
			else if  (reco[idi] < reco[idj])swap(rlist[i], rlist[j]);
		}
		
	}

	ret = Min(5, rcnt);

	F(i, 1, ret ) {
		list[i-1] = rlist[i];
	}

	
 	log("----ret = %d\n", ret);
	return ret ;
}
