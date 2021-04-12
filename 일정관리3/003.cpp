#define EE 1

#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d] " a "\n",__FUNCTION__, __LINE__,##__VA_ARGS)	
#else
#define log(a,...)
#endif

#define F(a,b,c)	for(register int a=b;a<=c;a++)

#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))
#define garolink(l,r) (l)->n=r;(r)->p=l
template<typename T> void swap(T &a, T&b) { T t = a; a = b; b = t; }

extern int initcnt ; 
extern int addEventcnt ;
extern int deleteEventcnt ; 
extern int getCountcnt ;
extern int changeEventcnt;

typedef unsigned long long ll;
#define MG 100
#define ME (100*1000)
#define HASH 207
#define MU 1000

class Event {
public:
	int uid;
	int gid;
	bool m;
	ll uv;
	ll lv;
	Event *n, *p; 	
}event[ME];
int gec;

class Grp {
public:	
	int cnt; 
	Event ghead[HASH], gtail[HASH];
}grp[MG];

class User{
public:
	int cnt; 
	Event uhead[HASH];
}user[MU]; 

void init()
{
	gec = 0;
	initcnt++;
	F(j, 0, HASH - 1) {
		F(i, 0, MG - 1) {
			grp[i].ghead[j].n = &grp[i].gtail[j];
			grp[i].gtail [j].p = &grp[i].ghead[j];
			grp[i].ghead[j].uv = grp[i].ghead[j].lv = 0; 
		}
		F(i, 0, MU - 1) {
			user[i].uhead[j].n = 0; 			
			user[i].uhead[j].uv = user[i].uhead[j].lv = 0; 
		}
	}
}

int getlong(char str[], Event * ep) {
	char *e = str; 
	ll ret = 0; 
	int h = 0; 

	F(i, 1, 8) {
		if (*e == '\0') {
			h = ret % HASH; 
			ep->uv = ret; 
			ret = 0; 
			break; 
		}
		ret = (ret << 5) + *e - 'a' + 1; 
		e++;
	}

	F(i, 1, 7) {
		if (*e == '\0') {
			h = ((h%HASH) + (ret% HASH))%HASH;
			ep->lv = ret;
			break;
		}
		ret = (ret << 5) + *e - 'a' + 1;
		e++;
	}
	return h; 
}

void addEvent(int uid, char ename[], int gid)
{
	addEventcnt++;

	// Event 를 만들고 
	Event * ep = &event[gec++];
	ep->gid = gid; 
	ep->uid = uid;
	ep->n = ep->p = 0; 

	// 해당 그룹에 추가를 하고 
	int gh, uh; 
	gh= uh = getlong(ename, ep );
	while (grp[gid].ghead[gh].uv != 0)
	{
		if (grp[gid].ghead[gh].uv == ep->uv && grp[gid].ghead[gh].lv == ep->lv) break; 
		else gh = (gh + 1) % HASH;
	}
	grp[gid].ghead[gh].uv = ep->uv;
	grp[gid].ghead[gh].lv = ep->lv;

	// master 인지 아닌지 결정. 
	if (grp[gid].ghead[gh].n == &grp[gid].gtail[gh]) {
		ep->m = true;
	}
	else ep->m = false;

	garolink(ep, grp[gid].ghead[gh].n); 
	garolink(&grp[gid].ghead[gh], ep);	

	// user 에 해당 
	while (user[uid].uhead[uh].uv != 0) {
		uh = (uh + 1) % HASH;
	}

	user[uid].uhead[uh].uv=  ep->uv; 
	user[uid].uhead[uh].lv = ep->lv;
	user[uid].cnt++; 
	user[uid].uhead->n = ep;
}

int deleteEvent(int uid, char ename[])
{
	deleteEventcnt++;
	Event temp = { 0, }; 

	int uh = getlong(ename, &temp);

	while (user[uid].uhead[uh].uv != temp.uv || user[uid].uhead[uh].lv != temp.lv) {
		uh = (uh + 1) % HASH;
	}

	int gid = user[uid].uhead[uh].n->gid; 
	if (user[uid].uhead[uh].n->m) {// master 니까 다 지워야 한다. 
		Event * ep = user[uid].uhead[uh].n; 
		Event * head = ep->p; 

		while (ep -> n !=0)
		{
			ep = ep->n; 
		}
		head->n = ep; 
		ep->p = head; 
		head->uv = head->lv = 0;
	}



	return 0;
}

int changeEvent(int uid, char ename[], char cname[])
{
	changeEventcnt++;
	return 0;
}

int getCount(int uid)
{
	getCountcnt++;
	return 0;
}