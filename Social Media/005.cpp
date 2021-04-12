#define EE 1

#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a "\n", __FUNCTION__,__LINE__,##__VA_ARGS__)
#else
#define log(a,...)
#endif

#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

template <typename T>
void swap(T &a, T&b) { T t = a; a = b; b = t; }

#define P 100000
#define L P
#define U 1000

int ctime = 0; 

class Post {
public:
	int pid; 
	int uid; 
	int like;
	int time;
	int operator < (Post pre) {
		if ((ctime - 1000) > time && (ctime - 1000) > pre.time) {
			if (like == pre.like) return time > pre.time; 
			else return like > pre.like; 
		}
		else if ((ctime - 1000) > time) {
			return 1; 
		}
		else if ((ctime - 1000) > pre.time) {
			return 0; 
		}
		else {
			return time > pre.time;
		}
	}
}post[P +1];

Post * newPost(int uid, int pid, int time) {
	Post * pp = &post[pid]; 
	pp->pid = pid; 
	pp->uid = uid; 
	pp->like = 0;
	pp->time = time; 
	return pp; 
}

int gl = 0; 
class Link {
public:
	Post * pp; 
	Link * n, *p; 
}link[L+1];

Link * newLink(Post * pp) {
	Link * lp = &link[gl++];
	lp->pp = pp; 
	lp->n = lp->p = 0; 
	return lp;
}

class User {
public:
	int uid;
	Link head, tail;
}user[U+1];

void init(int N)
{
	F(i, 0, U) {
		user[i].head.n = &user[i].tail;
		user[i].tail.p = &user[i].head;
		user[i].head.p = user[i].tail.n = 0;
	}
}

int fmap[U + 1][U + 1];

void follow(int uid1, int uid2, int time)
{
	int fcnt = ++fmap[uid1][0]; 
	log("++++ uid1= %d, uid2=%d time = %d fcnt = %d ", uid1, uid2, time, fcnt);
	fmap[uid1][fcnt] = uid2;

	fcnt = ++fmap[uid2][0];
	fmap[uid2][fcnt] = uid1;

	ctime = time; 
	log("----");  
}

void makePost(int uid, int pid, int time)
{
	log("++++uid = %d, pid = %d, time = %d", uid, pid, time	);
	Post * pp = newPost(uid, pid, time);
	Link * lp = newLink(pp); 

	User * up = &user[uid];	

	lp->n = up->head.n; 
	lp->p = &up->head; 

	if (up->head.n) up->head.n->p = lp; 
	up->head.n = lp;

	ctime = time;
	log("----");
}

void like(int pid, int time)
{
	log("++++ pid = %d, time = %d ", pid, time);
	post[pid].like++; 
	ctime = time;
	log("---- like = %d ", post[pid].like);
}

#define LM 10

class Ret {
public:
	int cnt;
	int list[LM + 1]; 
	void add(int n ) {
		list[cnt++] = n; 
		NF(i, cnt - 1, 1) {
			if ( post[ list[i] ] < post[list[i-1]])
		}




		cnt = Min(cnt, LM); 

	}

};

void getFeed(int uid, int time, int list[])
{
	log("++++");
	ctime = time;

	log("----");
}


