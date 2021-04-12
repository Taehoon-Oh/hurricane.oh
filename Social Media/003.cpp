#define EEE 0

#if EEE
#include <stdio.h>	
#define log(a,...)	printf ("%10s[%03d]: " a,__FUNCTION__,__LINE__,##__VA_ARGS__);
#else 
#define log(a,...)

#endif 

template <typename T>
void Swap(T &a, T &b) { T t = a; a = b; b = t; }

#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))


#define MP 100000
int gpc = 0; 
int mtime;
class Post {
public: 
	int pid; 
	int uid; 
	int like; 
	int time;

	int operator < ( Post pre) {
		if (mtime <= time && mtime <= pre.time) {

			if (like == pre.like) return time > pre.time;
			else return like > pre.like;
		}
		else if (mtime <= time)
		{
			return 1; 
		}
		else if (mtime <= pre.time)
		{
			return 0;
		}
		else
		{
			return time > pre.time;
		}
	}
} post[MP+1];

Post * newPost(int pid, int uid, int t) {
	Post * pp = &post[gpc++]; 
	pp->like = 0; 
	pp->pid = pid; 
	pp->uid = uid; 
	pp->time = t; 

	return pp; 
}

#define ML MP*20
int glc = 0; 

class Link {
public: 
	Post * pp; 
	Link * n;
	Link * p; 
}link[ML+1];
Link * newLink(Post * pp) {
	Link * lp = &link[glc++]; 
	lp->pp = pp; 
	lp->n = lp->p = 0; 

	return lp; 
}



#define MU 1000
class User {
public: 
	int uid;
	int cnt; 
	Link * head; 
	Link * tail; 
} user[MU+1];

int fmap[MU + 1][MU + 1];



void init(int N)
{
	log("++++ N = %d\n", N);

	F(i, 0, gpc - 1) post[i] = { 0, };
	F(i, 0, glc - 1) link[i] = { 0, };

	F(i, 0, MU) {
		user[i] = { 0, };
		F(j, 0, MU) fmap[i][j] = 0; 
		fmap[i][0] = 1, fmap[i][1] = i; 
	}

	gpc = 1; 
	glc = 0; 


	log("----   \n\n", );
}

void follow(int uid1, int uid2, int t)
{
	log("++++ uid1 = %d, uid2=%d\n", uid1, uid2);

	int cnt = ++fmap[uid1][0]; 
	fmap[uid1][cnt] = uid2; 


	log("----   \n\n", );
}

void makePost(int uid, int pid, int t)
{
	log("++++ uid = %d pid = %d, time = %d\n", uid, pid, t);

	Post *pp = newPost(pid, uid, t); 

	Link * lp = newLink(pp); 

	lp->n = user[uid].head; 
	user[uid].cnt++; 

	if (user[uid].head) user[uid].head->p = lp; 
	else user[uid].tail = lp; 

	user[uid].head = lp; 


	log("----  \n\n", );
}

void like(int pid, int t)
{
	post[pid].like++; 

}

#define MR 10

class Ret {
public:
	int cnt; 
	Post * list[MR + 1]; 

	void add(Post * pre) {
		list[cnt++] = pre; 

		NF(i, cnt - 1, 1) {
			if (*list[i] < *list[i - 1]) Swap(list[i], list[i - 1]); 
			else break; 
		}
		cnt = Min(cnt, MR); 
	}
};

Ret ret;
void getFeed(int uid, int t, int plist[])
{
	log("++++ uid = %d time = %d \n", uid, t);
	ret = { 0, };

	mtime = t - 1000; 

	int fcnt = fmap[uid][0]; 

	F(i, 1, fcnt) {
		int fid = fmap[uid][i]; 

		Link * lp = user[fid].head; 

		while (lp)
		{
			Post * pp = lp->pp; 
			ret.add(pp); 
			lp = lp->n; 
		}
	}

	F(i, 0, ret.cnt-1) plist[i] = ret.list[i]->pid; 

	log("---- \n\n");

}





