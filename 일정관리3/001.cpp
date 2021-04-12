
//void mstrcpy(char dst[], const char src[]) {
//	int c = 0;
//	while ((dst[c] = src[c]) != 0) ++c;
//}

//int mstrcmp(const char str1[], const char str2[]) {
//	int c = 0;
//	while (str1[c] != 0 && str1[c] == str2[c]) ++c;
//	return str1[c] - str2[c];
//}

#define EE 1

#if EE 
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]:" a "\n",__FUNCTION__,__LINE__,##__VA_ARGS__	)

#else
#define log(a,...)

#endif

#define F(a,b,c) for(register int a=b;a<=c;a++)

#define Max(a,b) (((a)>(b))?(a):(b))
#define Min(a,b) (((a)<(b))?(a):(b))

template <typename T>
void swap(T & a, T & b) { T t = a; a = b; b = t; }

template <typename T>
void deletelink(T& left, T& right) {
	left->n = right; 
	right->p = left; 
}



typedef unsigned long long ll; 

#define MAX (50000)

class Event {
public:
	int idx; 
	int uid; 
	int gid;
	int m; 
	ll name; 
	Event* n; 
	Event* p; 
}event[MAX+1];

int gec = 0; 

Event* newEvent(int uid, int gid, ll name) {
	Event* ep = &event[gec++]; 
	ep->idx = gec - 1; 
	ep->uid = uid; 
	ep->gid = gid; 
	ep->name = name; 
	ep->m = 0;
	ep->n = ep->p = 0; 
	return ep;
}

#define UMAX (1000)
#define GMAX (100)

Event uh[UMAX];
Event ut[UMAX];

class Grp {
public: 
	ll name;
	int cnt; 
	Event * ep[5];
	Grp* n; 
	Grp* p; 
}grp[20000] ;

int ggc = 0; 

Grp* newGrp(ll name) {
	Grp* gp = &grp[ggc++]; 
	gp->name =name; 
	gp->cnt = 0; 
	//gp->cp [0]= eid; 

	return gp; 
}

Grp gh[GMAX]; 
Grp gt[GMAX]; 

ll getlong(char name[]) {
	ll ret = 0; 

	char* s, * e, c; 
	s = e = name; 

	while (*e !='\0')
	{
		ret = ret << 5 + (*e - 'a'); 
		e++; 
	}
	return ret; 
}

int ucnt[UMAX];

void init()
{
	log("+++"); 
	gec = 0;
	ggc = 0; 

	F(i, 0, GMAX-1) {
		gh[i].n = &gt[i]; 
		gt[i].p = &gh[i]; 
		gh[i].p = gt[i].n = 0; 
	}
	F(i, 0, UMAX-1) {
		ucnt[i] = 0; 

		uh[i].n = &ut[i];
		ut[i].p = &uh[i];
		uh[i].p = ut[i].n = 0;
	}

	log("---\n"); 
}



void addEvent(int u, char name[], int g)
{
	log("+++ u= %d, name = %s, g =%d", u, name, g);

	ll lval = getlong(name); 
	log("long = %lld", lval); 

	Event* ep = newEvent(u, g, lval);

	ep->n = uh[u].n; 
	ep->n->p = ep; 
	uh[u].n = ep; 
	ep->p = &uh[u]; 
	ucnt[u]++; 

	log("ucnt[%d] = %d", u, ucnt[u]); 

	Grp* temp = gh[g].n; 

	while (temp != &gt[g]) {
		if (temp->name == lval) break; 
		temp = temp->n; 
	}

	if (temp->name == 0) {
		temp = newGrp(lval);
		temp->n = gh[g].n; 
		temp->n->p = temp; 

		temp->p = &gh[g]; 
		gh[g].n = temp; 

		ep->m = 1; // 해당 그룹에 동일 이름 없으니까  master 가 된다. 
	}

	temp->ep[temp->cnt++] = ep; 

	log("---\n");
}

int deleteEvent(int u, char name[])
{
	int ret = 0; 
	ll val = getlong(name); 
	log("+++ u=%d, name = %s  val = %lld", u , name	, val);

	Event* ep = uh[u].n; 

	while (ep != &ut[u])
	{
		log("ep->name = %lld ", ep->name); 

		if (ep->name == val)
		{			
			break; 
		}
		ep = ep->n; 
	}
	if (ep->n == 0) {
		return ret; 
	}	

	log("master = %d  ", ep->m);

	//deletelink(ep->p, ep->n); // 좌우를 이어 준다. 	
	

	int gid = ep->gid; 
	log("ucnt[%d]= %d  gid = %d ", u, ucnt[u], gid);

	Grp* temp = gh[gid].n;

	while (temp != &gt[gid]) {
		if (temp->name == val) break;
		temp = temp->n;
	}


	if (temp->name == 0) {
		log("null temp name ");
		return ret;
	}
	log("temp->name  = %lld  cnt = %d", temp->name, temp->cnt);

	if (ep->m == 1) {
		F(i, 0, temp->cnt - 1) {
			Event* tempep = temp->ep[i]; 
			int uid = tempep->uid; 
			deletelink(tempep->p, tempep->n); 
			log("uid = %d ucnt[%d]= %d  gid = %d ", uid, uid, ucnt[uid], gid);
			ucnt[uid]--; 
			log("uid = %d ucnt[%d]= %d  gid = %d ", uid, uid, ucnt[uid], gid);
			ret++; 
		}
		deletelink(temp->p, temp->n);  //  모든 일정을 지웠으니 다 그룹연결도 지운다. 

	}
	else
	{
		F(i, 0, temp->cnt - 1) {
			Event* tempep = temp->ep[i];
			if (tempep == ep )
			{
				temp->ep[i] = temp->ep[temp->cnt - 1]; 
				temp->cnt--; 
				int uid = tempep->uid;
				log("uid = %d ucnt[%d]= %d  gid = %d ", uid, ucnt[uid], gid);
				ucnt[uid]--;
				log("uid = %d ucnt[%d]= %d  gid = %d ", uid, ucnt[uid], gid);
				log("tempep = %p , ep = %p , temp->cnt = %d ", tempep, ep, temp->cnt); 
				ret++; 
				deletelink(tempep->p, tempep->n);
				break; 
			}
		}
	}
	

	log("---ret = %d \n", ret );
	return ret;
}

int changeEvent(int u, char name[], char cname[])
{
	int ret=0; 
	
	ll val = getlong(name);
	ll cval = getlong(cname);

	log("+++ u= %d, name = %s  %lld , cname = %s %lld ", u, name,val,  cname, cval);
	Event* ep = uh[u].n;

	while (ep != &ut[u])
	{
		log("ep->name = %lld ", ep->name);

		if (ep->name == val)
		{
			break;
		}
		ep = ep->n;
	}
	if (ep->name == 0) {
		log("null ep name "); 
		return ret;
	}

	log("master = %d  ", ep->m);

	ep->name = cval; 

	// 기존 그룹의 일정 이름으로 찾아가서 

	int gid = ep->gid; 
	log("gid = %d ", gid); 

	Grp* temp = gh[gid].n;

	while (temp != &gt[gid]) {
		log("group[%d] temp->name = %lld , temp->cnt ",gid,  temp->name, temp->cnt);
		if (temp->name == val) {			
			break;
		}
		temp = temp->n;
	}
	
	if (temp->name ==0)
	{
		log("null temp name ");
		return ret; 
	}

	if (ep->m) {
		temp->name = cval; 
		F(i, 0, temp->cnt - 1) {
			Event* tempep = temp->ep[i];
			log("group[%d] tempep->name = %lld  -> %lld", gid, tempep->name, cval);
			tempep->name = cval; 
			ret++;
		}
	}
	else
	{
		F(i, 0, temp->cnt - 1) {
			Event* tempep = temp->ep[i];
			log(" temp->ep[%d] = %p ", i, tempep);
			if (tempep == ep) { // 찾았다면 
				log(" temp->ep[%d] = %p , ep = %p", i, tempep, ep);
				temp->ep[i] = temp->ep[temp->cnt - 1];
				temp->cnt--;

				temp = newGrp(cval);
				temp->n = gh[gid].n;
				temp->n->p = temp;

				temp->p = &gh[gid];
				gh[gid].n = temp;

				ep->m = 1; // 마스터로 하라고 했다. 

				temp->ep[temp->cnt++] = ep;
				break; 
			}
		}	

		

	}



	log("---ret = %d \n", ret );
	return ret;
}

int getCount(int u)
{
	int ret = 0;
	ret = ucnt[u]; 
	log("+++ u=%d  ret = %d ", u, ret );

	log("---ret = %d \n", ret);
	return ret;
	
}