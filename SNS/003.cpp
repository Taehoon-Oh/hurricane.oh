#define EE 0

#include < vector>
#include <set>
#include <queue>

using namespace std; 

#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a "\n",__FUNCTION__,__LINE__,##__VA_ARGS__)


#else 
#define log(a,...)	
#define PP(a,...)	

#endif

#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)

template <typename T>
void swap(T & a, T & b) { T t = a; a = b; b = t; }

#define Max(a,b)	(((a)>(b))(a):(b))
#define Min(a,b)	(((a)<(b))(a):(b))



#define U      1000
#define H       5000
#define M		50000
#define MH		(2*M+7)
#define L (MH*10)

int mstrcmp(const char *a, const char *b);
void mstrcpy(char *dest, const char *src);

#define MC 10
#define MSTR 9
#define NFD -9999999

typedef unsigned long long ll;


#define MAX 500000

class Msg {
public: 
	int mid; 
	int uid; 
	int time; 
	ll tag[10]; 
}msg[MAX+1] ;

int gmc = 0; 

Msg* newMsg(int mid, int uid, int time, ll tag[]) {
	Msg* mp = &msg[gmc++]; 
	mp->mid = mid; 
	mp->uid = uid; 
	mp->time = time; 
	F(i, 0, 9) mp->tag[i] = tag[i]; 
	return mp; 
}


struct comp {
	bool operator () (const Msg &a, const Msg &b) const 
	{
		if (a.time == b.time)return b.uid > a.uid;
		else return b.time < a.time; 
	}
};
set < Msg, comp >  ss; 

#define MU  1000

int umap[MU + 1][MU + 1];

void init()
{
	F(i, 0, MAX) {
		msg[i].mid = msg[i].uid = msg[i].time = 0;
		F(j, 0, 9) msg[i].tag[j] = (ll)0; 
	}

	F(i, 0, MU) {
		umap[i][0] = 1; 
		umap[i][1] = i; 
	}

	ss.clear(); 
	
}



void createMessage(int mid, int uid, char msgs[])
{
	log("mid = %d uid =%d msg = %s",mid, uid, msgs ); 
	Msg m; 
	m.mid = mid; 
	m.uid = uid;
	m.time = ((msgs[0] - '0') * 10 + (msgs[1] - '0')) * 3600 + ((msgs[3] - '0') * 10 + (msgs[4] - '0')) * 60 + ((msgs[6] - '0') * 10 + (msgs[7] - '0'));

	char* s, * e, c; 
	s = e = &msgs[9]; 

	ll temp; 
	int index = 0; 

	while (1)
	{
		if (*e == '#') {
			s = ++e; 
		}
		else if (*e == ' ' || *e == '\0') {
			c = *e; 
			*e = '\0'; 
			temp = 0;

			while (s != e) {
				temp = ( temp + (ll)(*s- 'a')) << 5 ;
				s++; 
			}
			m.tag[index++] = temp; 

			*e = c;
			if (*e == '\0') break; 	

			s = ++e ; 

		}
		else e++; 
	}

	ss.insert(m);

	//for (set<Msg, comp> ::iterator iter = ss.begin(); iter != ss.end(); iter++) {
	//	log("iter.mid = %d", iter->mid); 
	//}

	/*for (set < Msg, comp > ::iterator iter = ss.begin(); iter != ss.end(); iter++) {
		log("iter->mid= %d", iter->mid); 
	}*/

}



void followUser(int u1, int u2)
{
	log("+++ u1 = %d, u2 = %d", u1, u2); 
	int &ucnt = ++umap[u1][0]; 
	umap[u1][ucnt] = u2; 

	log("ucnt = %d", ucnt); 
	log("---\n");
}

int searchByHashtag(char tag[], int ret[])
{
	log("+++ tag = %s	", tag);

	char* s, * e, c;
	s = e = &tag[0];

	ll temp = 0;	

	while (1)
	{
		if (*e == '#') {
			s = ++e;
		}
		else if (*e == ' ' || *e == '\0') {
			c = *e;
			*e = '\0';
			temp = 0;

			while (s != e) {
				temp = (temp + (ll)(*s - 'a')) << 5;
				s++;
			}
			*e = c;
			if (*e == '\0') break;

			s = ++e;

		}
		else e++;
	}
	log("tag = %lld", temp); 


	int retcnt = 0; 
	int size = ss.size(); 
	for (set<Msg, comp> ::iterator iter = ss.begin(); iter != ss.end(); iter++) {
		F(i, 0, 9) {
			if (iter->tag[i] == (ll)0) break;

			log("iter.tag[%d]= %lld",i, iter->tag[i]);
			if (iter->tag[i] == temp) {
				ret[retcnt++] = iter->mid; 
				log("ret[%d]= %d", retcnt-1, ret[retcnt-1]);
				if (retcnt == 5)
				{
					goto done; 
				}
			}
		}
	}

done: 

	log("---retcnt = %d \n", retcnt );	
	return retcnt;
}

int checkfollow(int base, int check) {
	int ret = 0; 
	int ucnt = umap[base][0];

	F(i, 1, ucnt) {
		if (umap[base][i] == check) {
			ret = 1; 
			break; 
		}
	}
	return ret; 
}

int getMessages(int u, int rets[])
{
	int ret = 0; 
	log("+++u = %d", u);
	

	for (set<Msg, comp> ::iterator iter = ss.begin(); iter != ss.end(); iter++) {
		int uu = iter->uid; 
		log("iter->uid = %d , iter->mid=%d", uu, iter->mid	); 
		int check = checkfollow(u, uu); 
		if (!check) continue; 
		rets[ret++] = iter->mid; 
		log("ret[%d] = %d ", ret - 1, rets[ret - 1]); 
		if (ret == 5) break; 
	}


	log("---ret = %d\n", ret);
	return ret;
}



int mstrcmp(const char *a, const char *b)
{
	int i;
	for (i = 0; a[i] != '\0'; ++i) if (a[i] != b[i]) return a[i] - b[i];
	return a[i] - b[i];
}

void mstrcpy(char *dest, const char *src)
{
	int i = 0;
	while (src[i] != '\0') { dest[i] = src[i]; i++; }
	dest[i] = src[i];
}