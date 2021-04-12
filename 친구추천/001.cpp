#define EE 0
#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define log(a,...)
#endif
#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)
#define Min(a,b)	(((a)<(b))?(a):(b))

template <typename T>
void swap(T&a, T &b) {
	T t = a; a = b; b = t;
}


#define ML	5
#define MF	10
#define MU 200

int fmap[MU+1][MU + 1];

int N;
class Ret {
public:
	int id;
	int val;
	int operator > (Ret pre) {
		if (val == pre.val) return id < pre.id;
		else return val > pre.val;
	}	

}ret[MU + 1];



void init(int n)
{
	N = n; 
	//F(i, 0, glc - 1) link[i] = { 0, }; 
	//F(i, 0, MU) user[i].cnt = 0, user[i].head.n= &user[i].tail; 

	F(i, 0, N) F(j, 0, N) fmap[i][j] = 0, ret[i] = { 0, };
}

void add(int id, int k, int ids[MF])
{
	F(i, 0, k - 1) {
		int cnt = ++fmap[id][0];
		int fid = ids[i]; 
		fmap[id][cnt] = fid; 

		log("id %d: cnt = %d, %d added", id, cnt, fid);

		cnt = ++fmap[fid][0]; 
		fmap[fid][cnt] = id; 
		log("id %d: cnt = %d, %d added", fid, cnt, id);

	}

}

void del(int id1, int id2)
{
	int cnt = fmap[id1][0];
	log("id %d: cnt = %d", id1, cnt);

	F(i, 1, cnt) {
		int fid = fmap[id1][i]; 
		if (fid == id2)
		{
			fmap[id1][i] = fmap[id1][cnt]; 
			fmap[id1][0]--; 
			log("id %d: cnt = %d, idx %d deled", id1, fmap[id1][0], i);
			break; 
		}
	}

	cnt = fmap[id2][0];
	log("id %d: cnt = %d", id2, cnt);

	F(i, 1, cnt) {
		int fid = fmap[id2][i];
		if (fid == id1)
		{
			fmap[id2][i] = fmap[id2][cnt];
			fmap[id2][0]--;
			log("id %d: cnt = %d, idx %d deled", id2, fmap[id2][0], i);
			break;
		}
	}
}



int recommend(int id, int list[5])
{
	log("++++ id = %d", id); 

	F(i, 1, N) ret[i].id = i, ret[i].val = 0;

	int cnt = fmap[id][0];
	log("++++ id = %d , fcnt = %d ", id, cnt);
	
	F(i, 1, cnt) {
		int fid = fmap[id][i];
		//ret[fid].val++; 
		log("fid %d , cnt %d", fid, fmap[fid][0]);

		//F(k, 1, N) printf("%d(%d) ", ret[k].id, ret[k].val);
		//printf("\n");

		int fcnt = fmap[fid][0]; 
		F(j, 1, fcnt) {
			int ffid = fmap[fid][j]; 
			ret[ffid].val++;
			log("id %d , cnt %d", ffid, fmap[ffid][0]);
		}
		//F(k, 1, N) printf("%d(%d) ", ret[k].id, ret[k].val);
		//printf("\n");
		//ret[id].val = 0, ret[fid].val = 0; 
	}

	ret[id].val = 0; 
	F(i, 1, cnt) {
		int fid = fmap[id][i];
		ret[fid].val = 0;
	}

	int scnt = 0;
	Ret sort[6] = { 0, };


	F(i, 1, N) {
		if (ret[i].val == 0)	continue; 

		sort[scnt++] = { ret[i].id, ret[i].val }; 
		NF(j, scnt - 1, 1) {
			if (sort[j] > sort[j - 1]) swap(sort[j], sort[j - 1]); 
			else break; 
		}
		scnt = Min(5, scnt); 
	}

	//F(k, 1, N) printf("%d(%d) ",ret[k].id, ret[k].val);
	//printf("\n");

	//quickSort(1, N , ret);

	//F(i, 1, N)printf("%d(%d) ", ret[i].id, ret[i].val);
	//printf("\n");

	cnt = 0; 
	F(i, 0, 4) {		
		if (sort[i].val == 0)	break; 

		list[i] = sort[i].id; 		
		cnt++; 
	}


	log("---- cnt = %d\n", cnt);
	return cnt;
}