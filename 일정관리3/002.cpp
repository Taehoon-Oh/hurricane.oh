#define EE 1
#include <vector>
#include <queue>
#include <set>

using namespace std; 


#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a "\n", __FUNCTION__,__LINE__,##__VA_ARGS__)

#else

#define log(a,...)

#endif

#define F(a,b,c)	for (register int a=b;a<=c;a++)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

template <typename T>
void swap(T & a, T & b) { T t = a; a = b; b = t; }

typedef unsigned long long ll; 

//void mstrcpy(char dst[], const char src[]) {
//	int c = 0;
//	while ((dst[c] = src[c]) != 0) ++c;
//}

//int mstrcmp(const char str1[], const char str2[]) {
//	int c = 0;
//	while (str1[c] != 0 && str1[c] == str2[c]) ++c;
//	return str1[c] - str2[c];
//}

#define EM 50000 
class Evt {
public: 
	int uid; 
	ll name; 
	int gid; 
	int m; 
	Evt* p, * n; 
}event[EM +1] ;

int gec = 0; 

Evt* newEvt(int uid, ll name, int gid) {
	Evt* ep = &event[gec++]; 
	ep->uid = uid; 
	ep->name = name; 
	ep->gid = gid; 
	ep->m = 0; 
	ep->n = ep->p = 0; 

	return ep; 
}


#define GM 200
class Gr {
public:
	ll name;
	int gid;
	int ecnt; 
	Evt* ep[5]; 
	Gr* n, * p; 
}gr[EM+1];

int ggc = 0; 

Gr* newGr(int gid, ll name) {
	Gr* gp = &gr[ggc++]; 
	gp->gid = gid; 
	gp->name = name; 
	gp->ecnt = 0; 
	gp->ep[0] = gp->ep[1] = gp->ep[2] = gp->ep[3] = gp->ep[4];
	return gp; 
}


ll getlong(char name[]) {
	ll ret = 0; 
	char* e = name; 

	while (*e !='\n')
	{
		ret = (ret << 5) + (*e - 'a'); 
		e++; 
	}

	return ret; 
}

int ucnt[1000];
Evt uhead[1000]; 
Evt utail[1000]; 

Gr ghead[100];
Gr gtail[100];
void init()
{
	log("+++ ");

	F(i, 0, EM) {
		event[i] = { 0, };
		gr[i] = { 0, };
	}

	F(i, 0, 999) {		
		ucnt[i] = 0; 
		
		uhead[i].n = &utail[i]; 
		utail[i].p = &uhead[i];
		uhead[i].p = utail[i].n = 0; 
	}
	
	F(i, 0, GM) {
		
		ghead[i].n = &gtail[i];
		gtail[i].p = &ghead[i];
		ghead[i].p = gtail[i].n = 0;
	}

	gec = 0;
	ggc = 0; 

	log("---  \n");
}

void addEvent(int u, char name[], int g)
{
	Evt* ep = newEvt(u, getlong(name), g); 
	log("+++ u = %d, name = %s (%lld)  g=%d", ep->uid , name, ep->name,ep->gid  );

	ep->n = uhead[u].n; 
	ep->p = &uhead[u]; 
	uhead[u].n->p = ep; 
	uhead[u].n = ep; 

	ucnt[u]++; 


	Evt* temp = uhead[u].n;
	F(i, 0, ucnt[u] - 1) {		
		log("%d: %d -> ", i, temp->uid); 
		temp = temp->n; 
	}


	Gr* gtemp = ghead[g].n; 

	while (gtemp != &gtail[g])
	{
		if (gtemp->name == ep->name)break; 
		gtemp = gtemp->n; 
	}
	// 같은 이름을 가진 gr 내의 원소를 찾는다. 
	

	
	if (gtemp->name == 0 ) { // 못찾았다면 
		Gr* gp = 0;
		gp = newGr(g, ep->name); 		
		gp->ecnt = 1; 
		gp->ep[0] = ep; 
		ep->m = 1;

		// 새로 해당 그룹에 추가 한다. 


		gp->n = ghead[g].n;
		gp->p = &ghead[g];

		gp->n->p = gp; 
		ghead[g].n = gp; 		
	}
	else
	{// 찾았다면 추가 한다.
		gtemp->ep[gtemp->ecnt++] = ep; 

		ep->m = 0; 
	}	


	log("---\n"); 
}
template <typename T >
void deletelink(T& left, T& right) {
	left->n = right; 
	right->p = left; 
}

int deleteEvent(int u, char name[])
{
	int ret = 0; 
	log("+++ u = %d, name = %s, ", u, name);

	ll lval = getlong(name); 

	Evt * etemp = uhead[u].n;

	while (etemp != &utail[u])
	{
		if (etemp->name == lval)break;
		etemp = etemp->n;
	}

	if (etemp->name == 0) return ret; 

	int master = etemp->m; 
	log("etemp->name = %lld , m= %d ", etemp->name, master	); 

	Gr* gtemp = ghead[etemp->gid].n; 

	while (gtemp != &gtail[etemp->gid])
	{
		if (etemp->name == lval){
			log("found in group %lld", etemp->name); 
			break; 
		}
	}
	if (gtemp->name == 0) {
		log("not found in group ");
		return ret; 
	}

	if (master) { 
		F(i, 0, gtemp->ecnt - 1) {
			Evt* ep = gtemp->ep[i]; 
			deletelink(ep->p, ep->n);
			ucnt[ep->uid]--; 
			log("ucnt[%d]= %d", ep->uid, ucnt[ep->uid]); 
			ret++; 
		}
		deletelink(gtemp->p, gtemp->n);
	}
	else
	{
		F(i, 0, gtemp->ecnt - 1) {
			Evt* ep = gtemp->ep[i];
			if (ep->name == lval) {
				deletelink(ep->p, ep->n);
				ucnt[ep->uid]--;
				log("ucnt[%d]= %d", ep->uid, ucnt[ep->uid]);
				ret++;
				break; 
			}
		}

	}

	log("--- ret = %d \n", ret );
	return ret; 
}

int changeEvent(int u, char name[], char cname[])
{
	int ret = 0;
	

	ll lval = getlong(name);
	ll clval = getlong(cname);
	log("+++ u = %d, name = %s, %lld  cname=%s, %lld", u, name, lval, cname, clval);

	Evt* etemp = uhead[u].n;

	while (etemp != &utail[u])
	{
		if (etemp->name == lval)break;
		etemp = etemp->n;
	}

	if (etemp->name == 0) return ret;

	int master = etemp->m;

	int gid = etemp->gid; 
	log("etemp->name = %lld , m= %d , gid = %d ", etemp->name, master, gid);

	Gr* gtemp = ghead[gid].n;

	while (gtemp != &gtail[gid])
	{
		log("name =  %lld", gtemp->name);
		if (gtemp->name == lval) {
			log("found in group %lld", gtemp->name);
			break;
		}
		gtemp = gtemp->n;
	}
	if (gtemp->name == 0) {
		log("not found in group ");
		return ret;
	}

	if (master) {
		F(i, 0, gtemp->ecnt - 1) {
			Evt* ep = gtemp->ep[i];
			//deletelink(ep->p, ep->n);
			//ucnt[ep->uid]--;
			//log("ucnt[%d]= %d", ep->uid, ucnt[ep->uid]);
			ep->name = clval;
			ret++;
		}
		gtemp->name = clval; 
	}
	else
	{
		F(i, 0, gtemp->ecnt - 1) {
			Evt* ep = gtemp->ep[i];
			if (ep->name == lval) {
				//deletelink(ep->p, ep->n);
				//ucnt[ep->uid]--;
				//log("ucnt[%d]= %d", ep->uid, ucnt[ep->uid]);
				gtemp->ep[i] = gtemp->ep[gtemp->ecnt - 1]; 
				gtemp->ep[gtemp->ecnt - 1] = 0; 
				gtemp->ecnt--; 
				ret++;				

				Gr* gp = newGr(gid, clval);
				gp->ecnt = 1; 
				gp->ep[0] = ep; 

				gp->n = ghead[gid].n;
				gp->p = &ghead[gid];

				gp->n->p = gp;
				ghead[etemp->gid].n = gp;
				break;
			}
		}
	}

	log("--- ret = %d \n", ret);
	return ret;
}

int getCount(int u)
{
	int ret = 0;
	log("+++ u = %d", u);

	ret = ucnt[u]; 

	log("--- ret = %d \n", ret);
	return ret;
}

