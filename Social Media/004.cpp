#define EE 0

#if EE
#include <stdio.h>
#define log(a,...) printf("%10s[%03d]: " a "\n",__FUNCTION__,__LINE__,##__VA_ARGS__)
#else
#define log(a,...)
#endif

#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

template <typename T>
void Swap(T&a, T&b) { T t = a; a = b; b = t; }

#define MU 1000
#define MP 100000
int ctime=0; 

int gpc = 1; 
class Post {
public: 
	int pid, l, t;
	int operator < (Post &pre) {
		if (t >= ctime && pre.t >= ctime) {
			if (l == pre.l) return t > pre.t; 
			return l > pre.l; 
		}
		else if (t >= ctime) return 1; 
		else if (pre.t >= ctime)return 0; 
		else return t > pre.t; 

	}
}post[MP+1];

Post * newPost(int pid ,int uid, int time) {
	Post * pp = &post[pid]; 
	pp->pid = pid; 
	pp->t = time; 
	pp->l = 0; 
	gpc = pid + 1; 
	
	return pp; 
}

int glc = 0; 
#define ML (MP)
class Link {
public: 
	Post * pp; 
	Link * n; 
}link[ML+1];

Link * newLink(Post * pp) {
	Link * lp = &link[glc++]; 
	lp->pp = pp; 
	lp->n = 0; 
	return lp; 
}

class User {
public:
	Link *head; 	
	int cnt; 
}user[MU+1];

int fmap[MU + 1][MU + 1];


void init(int N)
{
	log("++++");
	F(i, 0, gpc - 1)post[i] = { 0, }; 
	F(i, 0, glc - 1)link[i] = {0, };
	F(i, 0, MU) user[i] = { 0, };
	F(i, 0, MU){
		F(j, 0, MU)fmap[i][j] = 0; 
		fmap[i][0] = 1, fmap[i][1] = i; 
	}
	glc = 0; 

	log("----\n");
}

void follow(int i1, int i2, int time)
{
	log("++++uid1=%d, uid2=%d",i1,i2);
	int cnt = ++fmap[i1][0];
	fmap[i1][cnt] = i2; 

	log("----uid %d: cnt = %d, %d added \n",i1, cnt, i2);
}

void makePost(int uid, int pid, int time)
{
	log("++++ uid=%d, pid=%d, time =%d",uid, pid, time);
	
	Post* pp = newPost(pid, uid, time);

	Link * lp = newLink(pp); 
	
	lp->n = user[uid].head; 	
	user[uid].cnt++; 
	user[uid].head = lp; 

	log("uid %d: cnt =%d pid %d added", uid, user[uid].cnt, pid); 

	log("----\n");
}

void like(int pid, int time)
{
	log("++++ pid %d",pid);	
	post[pid].l++; 
	log("----pid %d, like = %d\n",pid, post[pid].l);
}

#define MR 10
class Ret {
public:
	int cnt; 
	int list[MR+1];
	void init() {
		F(i, 0, MR) list[i] = 0; 
		cnt = 0; 
	}
	void add(int n) {
		list[cnt++] = n;

		NF(i, cnt - 1, 1) {
			if (post[list[i]] < post[list[i - 1]]) Swap(list[i], list[i - 1]); 
			else break; 
		}
		cnt = Min(MR, cnt); 
	}
}ret;

#define MAX_SIZE (MP+1)

Post * heap[MAX_SIZE];
int heapSize = 0;

void heapInit(void)
{
	heapSize = 0;
}

int heapPop(Post * &value);
int heapPush(Post * value);

void getFeed(int uid, int time, int list[])
{
	//ret.init(); 
	heapInit(); 
	log("++++ uid %d, time =%d", uid, time);
	ctime = time - 1000; 
	log("ctime = %d", ctime); 

	int fcnt = fmap[uid][0];
	log("fcnt = %d", fcnt); 

	Link * stop[MU + 1] = { 0, }; 	
	bool more = false; 

	F(i, 1, fcnt) {
		int fid = fmap[uid][i]; 
		log("fid = %d ", fid); 

		Link * lp = user[fid].head; 
		
		if (lp == 0) continue; 
		//int cnt = MR; 
		//while (lp  && cnt--) {
		
		while (lp  ) {
			//Link * next = lp->n; 
			if (lp->pp->t < ctime) {
				stop[fid] = lp; 
				more = true;
				break; 
			
			}

			//ret.add(lp->pp->pid); 
			heapPush(lp->pp);
			log("cnt = %d : 1(pid =%d ), 2(pid=%d ), 3(pid=%d ),4( pid=%d ),5( pid=%d ),6(pid=%d ),7(pid=%d ),8(pid=%d ),9(pid=%d ),10(pid=%d )", ret.cnt, ret.list[0], ret.list[1], ret.list[2], ret.list[3], ret.list[4], ret.list[5], ret.list[6], ret.list[7], ret.list[8], ret.list[9]);

			lp = lp->n; 
		}
	}
	if (more && heapSize < 10) {
		int left = 10 - heapSize;

		F(i, 1, fcnt) {
			int fid = fmap[uid][i];
			log("fid = %d ", fid);

			Link * lp = stop[fid];
			int len = 0; 

			//if (lp == 0) continue;
			//int cnt = MR; 
			//while (lp  && cnt--) {

			while (lp) {
				//Link * next = lp->n; 
				if (len == left) {
					// 더 넣어야 하는 만큼만 넣는다. 
					break;
				}

				//ret.add(lp->pp->pid);
				heapPush(lp->pp);
				log("cnt = %d : 1(pid =%d ), 2(pid=%d ), 3(pid=%d ),4( pid=%d ),5( pid=%d ),6(pid=%d ),7(pid=%d ),8(pid=%d ),9(pid=%d ),10(pid=%d )", ret.cnt, ret.list[0], ret.list[1], ret.list[2], ret.list[3], ret.list[4], ret.list[5], ret.list[6], ret.list[7], ret.list[8], ret.list[9]);

				lp = lp->n;
				len++; 
			}
		}
	}

	Post * pp = 0; 
	int i = 0; 
	while ( heapPop(pp)) {
		list[i++] = pp->pid;
		if (i == 10)break; 
	}

	log("----\n");
}



int heapPush(Post * value)
{
	if (heapSize + 1 > MAX_SIZE)
	{
		//printf("queue is full!");
		return 0;
	}

	heap[heapSize] = value;

	int current = heapSize;
	while (current > 0 && *heap[current] < *heap[(current - 1) / 2])
	{
		Post * temp = heap[(current - 1) / 2];
		heap[(current - 1) / 2] = heap[current];
		heap[current] = temp;
		current = (current - 1) / 2;
	}

	heapSize = heapSize + 1;

	return 1;
}

int heapPop(Post * &value)
{
	if (heapSize <= 0)
	{
		return 0;
	}

	value = heap[0];
	heapSize = heapSize - 1;

	heap[0] = heap[heapSize];

	int current = 0;
	while (current * 2 + 1 < heapSize)
	{
		int child;
		if (current * 2 + 2 == heapSize)
		{
			child = current * 2 + 1;
		}
		else
		{
			child = *heap[current * 2 + 1] < *heap[current * 2 + 2] ? current * 2 + 1 : current * 2 + 2;
		}

		if (*heap[current] < *heap[child])
		{
			break;
		}

		Post * temp = heap[current];
		heap[current] = heap[child];
		heap[child] = temp;

		current = child;
	}
	return 1;
}

