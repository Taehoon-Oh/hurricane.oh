#define EE 1

#if EE
#include <stdio.h>
#define log(a,...)	 printf("%10s[%03d]: " a "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#else 
#define log(a,...)
#endif 

#define F(a,b,c)	 for(register int a=b;a<=c;a++)
#define Max(a,b)	(((a)>(b))?(a):(b))

template <typename T>
void swap(T &a, T &b) { T t = a; a = b; b = t;  }

template <typename T>
void garolink(T &l, T &r) { l->n = r; r->p = l; }


#define MI	100001
#define MU 1001

class Item {
public: 
	int st; 
	int rt; 
	int sid; 
	int rid; 
	int mid;
	int sread;
	int sdel;
	int rread;
	int rdel;
}item[MI];

int gic = 0;

Item * newItem(int st, int sid, int rid, int mid, int rt) {
	Item * ip = &item[gic++]; 
	ip->st = st;
	ip->rt = rt; 
	ip->sid = sid; 
	ip->rid = rid; 
	ip->mid = mid; 
	ip->rread = ip->sread = ip->sdel=ip->rdel= 0; 
	return ip;
}
int glc; 

class Link {
public: 
	Item *ip; 
	Link *n, *p; 
}link[MI];

Link * newLink(Item * ip) {
	Link * lp = &link[glc++];
	lp->ip = ip; 
	lp->n = lp->p = 0; 
	return lp; 
}

Link ihead, itail;


class Sent {
public:
	Item * ip;
	Sent * n, *p; 
}sent[MI];
int gsc = 0;

Sent * newSent(Item * ip) {
	Sent * sp = &sent[gsc++];
	sp->ip = ip;
	sp->n = sp->p = 0;
	return sp;
}

Sent shead[MU], stail[MU]; 


class Recv {
public:
	Item *ip; 
	Recv *n, *p; 
}recv[MI];
int grc = 0;

Recv * newRecv(Item * ip) {
	Recv * rp = &recv[grc++];
	rp->ip = ip;
	rp->n = rp->p = 0;
	return rp;
}


Recv rhead[MU], rtail[MU];
int ctime;

void init(int N)
{
	log("+++ N=%d", N); 
	ihead.n = &itail; 
	itail.p = &ihead; 

	F(i, 0, MU - 1) {
		shead[i].n = &stail[i]; 
		stail[i].p = &shead[i];

		rhead[i].n = &rtail[i];
		rtail[i].p = &rhead[i]; 
	}
	gic = glc = gsc=grc=0; 

	ctime = 0; 

	log("---\n"); 

}



void job(int ct) {
	// Item 의 할일 
	ctime = ct; 

	Link * lgaro = ihead.n; 

	while (lgaro != &itail)
	{
		Item * ip = lgaro->ip; 
		if (ip->rt > ctime) { // 지금 시간의 다음 일은 하지 않는다. 
			break; 
		}

		if (ip->sdel == 1) {
			garolink(lgaro->p, lgaro->n); 
			lgaro = lgaro->n; 
			continue; 
		}

		Recv * rp = newRecv(ip);
		int sid = ip->sid; 
		int rid = ip->rid;

		log("recv: mid = %d rp->ip->rt = %d", ip->mid, rp->ip->rt);

		// Recv 순서에 맞게 넣자. 
		Recv * rgaro = rtail[rid].p; // 받는 사람의 칸에 넣어줄 준비를 한다. 
		int cnt = 0; 
		while (rgaro != &rhead[rid]) {
			Item * itemp = rgaro->ip;			
			
			if (item->rt < ip->rt) {
				log("recv: item->t = %d, ip->t = %d", itemp->rt, ip->rt);
				break;
			}
			rgaro = rgaro->p;
			cnt++; 
			if ( cnt ==3) break; 
		}
		if (cnt != 3) {
			garolink(rp, rgaro->n);
			garolink(rgaro, rp);
		}

		garolink(lgaro->p, lgaro->n); // 방금 처리한 것은 지워버린다. 다시 하지 않도록 
		lgaro = lgaro->n; 
	}

}

void sendMessage(int st, int uid1, int uid2, int mid, int rt)
{
	log("+++ st=%d, u1=%d, u2=%d, mid=%d, rt=%d", st, uid1, uid2, mid, rt); 

	Item *ip = newItem(st, uid1, uid2, mid, rt);

	Link * lp = newLink(ip); 
	Sent * sp = newSent(ip);	

	// Item 순서에 맞게 넣자. 
	Link * lgaro = ihead.n; 

	while (lgaro != &itail ) {
		Item * itemp = lgaro->ip; 
		//log("link: itemp->rt = %d, in->rt = %d", itemp->rt, ip->rt);
		//log("link: lgaro->ip->rt = %d, in->rt = %d", lgaro->ip->rt, ip->rt);

		if (itemp->rt > ip->rt) {
			log("link: pre->rt = %d, in->rt = %d", itemp->rt, ip->rt);
			break; 
		}
		lgaro = lgaro->n; 
	}
	/*lgaro->p->n = lp; 
	lp->p = lgaro->p; 
*/
	garolink(lgaro->p, lp); 
	garolink(lp, lgaro); 


	// Sent 순서에 맞게 넣자. 
	Sent * sgaro = stail[uid1].p;
	int cnt = 0; 

	while (sgaro != &shead[uid1]) {
		Item * itemp = sgaro->ip;

		if (itemp->st < ip->st) {
			log("sent: item->t = %d, ip->t = %d", itemp->st, ip->st);
			break;
		}
		sgaro = sgaro->p;
		cnt++; 
		if (cnt == 3) break; 
	}
	if (cnt != 3) {
		garolink(sp, sgaro->n);
		garolink(sgaro, sp);
	}

	job(st); 

	log("---\n"); 
}

int retrieveSentbox(int ct, int uid, int mlist[3], int ulist[3], int rlist[3])
{
	ctime = ct; 
	int ret = 0;
	log("+++ ct=%d, uid=%d", ct, uid);

	job(ct);

	Sent * sp = stail[uid].p; 


	while (sp != &shead[uid])
	{
		mlist[ret] = sp->ip->mid; 
		ulist[ret] = sp->ip->rid; 
		rlist[ret] = sp->ip->rread; 
		ret++; 
		sp = sp->p; 
		if (ret == 3)
		{
			break; 
		}
	}

	log("---ret = %d \n", ret);
	return ret;
}

int retrieveInbox(int ct, int uid, int mlist[3], int ulist[3], int rlist[3])
{
	int ret = 0;
	log("+++ ct=%d, uid=%d", ct, uid);

	job(ct);

	Recv * rp = rtail[uid].p;


	while (rp != &rhead[uid])
	{
		mlist[ret] = rp->ip->mid;
		ulist[ret] = rp->ip->sid;
		rlist[ret] = rp->ip->rread;
		ret++;
		rp = rp->p;
		if (ret == 3)
		{
			break;
		}
	}

	log("---ret = %d \n", ret);
	return ret;
}

int readMessage(int ct, int uid, int mid)
{
	int ret = 0; 
	log("+++ ct=%d, uid=%d mid = %d", ct, uid, mid);
	job(ct);

	Sent * sp = shead[uid].n;
	while (sp != &stail[uid])
	{
		if (sp->ip->mid == mid )
		{
			ret = 1;
			sp->ip->sread = 1; 
			break;
		}		
		sp = sp->n;		
	}

	Recv * rp = rhead[uid].n;
	while (rp != &rtail[uid])
	{
		if (rp->ip->mid == mid)
		{
			rp->ip->rread = 1; 
			ret = 1; 
			break;
		}		
		rp = rp->n;		
	}
	

	log("---ret = %d \n", ret);
	return ret;
}

int deleteMessage(int ct, int uid, int mid)
{
	int ret = 0;
	log("+++ ct=%d, uid=%d mid= %d", ct, uid, mid);

	job(ct);

	Sent * sp = shead[uid].n;
	while (sp != &stail[uid])
	{
		if (sp->ip->mid == mid)
		{
			ret = 1;
			sp->ip->sdel = 1; 
			garolink(sp->p, sp->n); 
			break;
		}
		sp = sp->n;
	}

	Recv * rp = rhead[uid].n;
	while (rp != &rtail[uid])
	{
		if (rp->ip->mid == mid)
		{
			ret = 1;
			rp->ip->rdel = 1; 
			garolink(rp->p, rp->n);
			break;
		}
		rp = rp->n;
	}

	log("---ret = %d \n", ret);
	return ret;
}