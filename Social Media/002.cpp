#define DBG 0

#if DBG
#include <stdio.h>
#define log(a,...)	printf ( "%10s[%03d]: " a,__FUNCTION__,__LINE__,##__VA_ARGS__)

#else
#define log(a,...)

#endif

#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

#define  MP 100000

int ctime; 

class Post {
public:
	int pid;
	int like;
	int time;

	int operator < (Post pre) {
		if (time >= ctime - 1000 && pre.time >= ctime - 1000) {
			if (like == pre.like) return time > pre.time; 
			else return like > pre.like; 
		}
		else if (time >= ctime - 1000)
		{
			return 1; 
		}
		else if (pre.time >= ctime - 1000)
		{
			return 0; 
		}
		else
		{
			return time > pre.time; 
		}		
	};
};

int gpc = 0;
Post post[MP + 1];

Post * newPost(int pid, int t) {
	Post *pp = &post[pid]; 
	pp->pid = pid;
	pp->like = 0;
	pp->time = t; 

	return pp; 
}

#define ML MP
class Link {
public:
	Post * pp; 
	Link * n;
	Link * p; 
};
Link link[ML + 1]; 
int glc = 0;
Link * newLink(Post * pp) {
	Link * lp = &link[glc++]; 
	lp->pp = pp; 
	lp->n = lp->p = 0; 

	return lp; 
}


#define MU 1000
class User {
public: 	
	int flist[MU + 1]; 
	int lcnt;
	Link * t; 
	Link * h;
};

User user[MU + 1]; 

template <typename T>
void Swap(T &a, T&b) {
	T t=a; 
	a = b;
	b = t; 
}


void init(int N)
{
	log("++++ N = %d\n", N); 
	F(i, 0, MP) post[i] = { 0, };// {{0,}; 
	F(i, 0, glc-1) link[i] = { 0, }; 
	F(i, 0, MU) user[i] = { 0, }, user[i].flist[0]=1, user[i].flist[1]=i;

	glc = 0;
	gpc = 0; 

	log("----   \n\n", );
}

void follow(int uid1, int uid2, int t)
{
	log("++++ uid1 = %d, uid2=%d\n", uid1, uid2);
	int * flist = user[uid1].flist;

	int cnt = ++flist[0];
	flist[cnt] = uid2;

	log("cnt = %d  list:", cnt);
#if DBG
	F(i, 1, cnt) printf("%d -> ", flist[i]);
	printf("\n");
#endif 
}

void makePost(int uid, int pid, int t)
{
	log("++++ uid = %d pid = %d, time = %d\n", uid, pid, t);

	Post * pp = newPost(pid, t);
	Link * lp = newLink(pp); 

	User * u = &user[uid];
	
	lp->n = u->h; 

	if (u->t == 0) u->t = lp; 
	else  u->h->p = lp; 

	u->h = lp; 
	u->lcnt++; 

#if DBG
	log("uid %d: lcnt = %d : ", uid, u->lcnt); 
	while (lp	)
	{
		printf("%d -> ", lp->pp->pid); 
		lp = lp->n; 
	}
	printf("\n"); 
#endif 

	log("----  \n\n", );
}

void like(int pid, int t)
{
	log("++++ pid = %d\n", pid);

	post[pid].like++; 

	log("---- like = %d\n\n",post[pid].like );
	
}

#define MR 10

class Ret {
public: 
	int cnt; 
	int list[MR + 1]; 
	void init() {
		F(i, 0, MR) list[i] = 0; 
		cnt = 0; 
	}; 

	void add(int in) {
		list[cnt++] = in; 

		NF(i, cnt - 1, 1) {
			log("list i : time %d like %d   -  list i-1: time %d  like  %d \n", post[list[i]].time, post[list[i]].like , post[list[i-1]].time, post[list[i-1]].like);
			if (post[list[i]] < post[list[i - 1]]) {
				Swap(list[i], list[i - 1]); 
			}
			else  break; 
		}
		cnt = Min(cnt, MR); 
		return; 
	}; 
};

Ret ret; 
void getFeed(int uid, int t, int plist[])
{
	log("++++ uid = %d time = %d \n", uid, t);
	ret.init();
	ctime = t;

	int fcnt = user[uid].flist[0];


	F(i, 1, fcnt) {
	int fid = user[uid].flist[i]; 
	log("fid = %d \n", fid); 
	Link * lp = user[fid].h;

	int cnt = 1; 
	while (lp && cnt < 11)
	{
		int pid = lp->pp->pid;
		log("pid = %d , time %d like %d \n", lp->pp->pid, lp->pp->time, lp->pp->like); 

		if (post[ret.list[cnt]].time < post[pid].time || post[ret.list[cnt]].like < post[pid].like || cnt < 10 )
		{
			ret.add(pid);
		}
		lp = lp->n;
	}
}

	F(i, 0, MR - 1) plist[i] = ret.list[i]; 
	log("---- \n\n");
	
}
