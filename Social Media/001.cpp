#define DBG 0


#if DBG 
//#include <stdio.h>
#define log(a,...)  printf("%10s[%03d]:" a, __FUNCTION__,__LINE__,##__VA_ARGS__)
#else
#define log(a,...)
#endif 

#define F(a,b,c)	for(register int a=b; a<=c;a++)
#define NF(a,b,c)	for(register int a=b; a>=c;a--)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

template <typename T>
void  Swap(T &a, T &b) {
	T temp = a;
	a = b;
	b = temp;
	return;
}

#define MP 100000
int ctime = 0;
class Post {
public:
	int pid;
	//int idx;
	//int uid;
	int time;
	int like;

	int operator < (Post pre) {
		if (time >= ctime - 1000 && pre.time >= ctime - 1000) {
			if (like == pre.like)return time > pre.time;
			else return like > pre.like;
		}
		else if (time >= ctime - 1000)
		{
			return 1;
		}
		else if (pre.time > ctime - 1000)
		{
			return 0;
		}
		else
		{
			return time > pre.time;
		}
	}
};
int gpc = 0;
Post post[MP + 1];
Post * newPost(int pid, int uid, int time) {
	Post * pp = &post[gpc++];
	pp->pid = pid;
	//pp->uid = uid;
	pp->time = time;
	pp->like = 0;
	//pp->idx = gpc - 1;

	return pp;
}

#define ML MP

class Link {
public:
	//int lid;
	Post * pp;
	Link * next;
	//Link * prev;
};

Link link[ML + 1];
int glc = 0;
Link * newLink(Post * pp) {
	Link * lp = &link[glc++];
	lp->pp = pp;
	lp->next = 0;
	//lp->lid = glc - 1;

	return lp;
}

#define MU 1000

class User {
public:
	//int uid;
	int fcnt;
	int flist[MU + 1];
	//Link * last;
	Link * start;
	//int lcnt;
};

User user[MU + 1];



void init(int N)
{
	log("++++ N=%d	\n", N);

	F(i, 1, gpc - 1) post[i] = { 0, };
	F(i, 0, glc - 1)link[i] = { 0, };
	F(i, 0, MU) user[i] = { 0, }, user[i].fcnt = 1, user[i].flist[0] = i;

	gpc = 1;
	glc = 0;
	log("----\n\n");
	return;
}

void follow(int uid1, int uid2, int time)
{
	log("++++ uid1=%d uid2=%d time = %d \n", uid1, uid2, time);

	int cnt = ++user[uid1].fcnt;
	user[uid1].flist[cnt - 1] = uid2;

	log("----uid %d: fcnt = %d, added (%d)	\n\n", uid1, cnt, uid2);
	return;
}

void makePost(int uid, int pid, int time)
{
	log("++++ uid =%d pid = %d time = %d\n", uid, pid, time);

	Post * pp = newPost(pid, uid, time);

	Link * lp = newLink(pp);


	//user[uid].lcnt++;
	lp->next = user[uid].start;
	//if (user[uid].start == 0) user[uid].last = lp;
	//else user[uid].start->prev = lp;

	user[uid].start = lp;

	log("----\n\n");
	return;
}

void like(int pid, int time)
{
	log("+++ pid %d	time = %d	\n", pid, time);

	int like = ++post[pid].like;

	log("----pid %d : like = %d \n\n", pid, like);
}

#define MR 10
class Result {
public:
	int cnt;
	int list[MR + 1];
	void init() {
		F(i, 0, MR) list[i] = 0;
		cnt = 0;
	}

	void add(int in) {
		list[cnt++] = in;
		NF(i, cnt - 1, 1) {
			if (post[list[i]] < post[list[i - 1]]) {
				Swap(list[i], list[i - 1]);
			}
			else
			{
				break;
			}
		}
		cnt = Min(cnt, MR);
	}
};
Result ret;
void getFeed(int uid, int time, int plist[])
{
	log("++++ uid =%d	time = %d \n", uid, time);
	ctime = time;
	ret.init();

	int fcnt = user[uid].fcnt;
	log("fcnt = %d\n", fcnt);
	F(i, 0, fcnt - 1) {
		int fid = user[uid].flist[i];

		log("fid = %d\n", fid);
		Link *lp = user[fid].start;

		int cnt = 1;
		while (lp && cnt != 11)
		{
			ret.add(lp->pp->pid);
			lp = lp->next;
			cnt++;
		}
	}

	F(i, 0, 9) plist[i] = ret.list[i];

	log("----\n\n");
	return;
}