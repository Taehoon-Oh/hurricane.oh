#define EE 1

#if EE
#include <stdio.h>
#define log(a,...)	 printf("%10s[%03d] " a "\n", __FUNCTION__,__LINE__,##__VA_ARGS__)
#else
#define log(a,...)
#endif

#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define Max(a,b,c)	(((a)>(b))?(a):(b))
#define Min(a,b,c)	(((a)<(b))?(a):(b))

template <typename T> void swap(T &a,T&b){T t = a; a = b; b = t;}
#define garolink(l,r) (l)->n=r;(r)->p=l
typedef unsigned long long ll;

#define MAX 10000
#define HASH 20007
#define MU 1000
#define MK 300

class Mail{
public:
	int vcnt; 
	ll val[10];
	bool operator == ( Mail in) {
		if (vcnt != in.vcnt) return false;
		F(i, 0, vcnt-1) {
			if (val[i] != in.val[i]) return false;
		}
		return true;
	}
}mail[MAX + 1];
int gmc = 0;

class User {
public:
	int num;
	int start; 
	int end; 
	int list[MK + 1];
}user[MU+1];

class Link {
public:
	ll val;
	int mid;
	Link *n, *p; 
}link[MAX*10+1];
int glc = 0;

Link hashhead[HASH], hashtail[HASH];

int N, K;
void init(int n, int k)
{
	N = n, K = k;
	log("++++ N=%d, K= %d\n",N,K);

	F(i, 0, HASH - 1) {
		hashhead[i].n = &hashtail[i];
		hashtail[i].n = &hashhead[i];
		hashhead[i].val = 0; 
	}

	F(i, 0, MU) {
		user[i] = { 0, }; 
	}
	gmc = glc = 0;

	log("----\n\n");
}

void getlong(char str[], Mail * mp) {
	
	char *c = &str[0];
	ll ret = 0; 

	while (1) {
		if (*c == 0) {
			mp->val[mp->vcnt++]=ret;
			break;
		}
		else if (*c == ' ') {
			mp->val[mp->vcnt++] = ret;
			ret = 0;
		}
		else ret = (ret << 5) + *c - 'a' + 1;
		c++; 
	}
	return ; 
}


void sendMail(char str[], int sid, int rc, int rids[])
{
	log("++++ str = %s, rc =%d", str, rc);
	mail[gmc] = { 0, }; 
	getlong(str, &mail[gmc]);

	F(i, 0, mail[gmc].vcnt - 1) {
		//log("temp [%d] =%lld ", i, mail[gmc].val[i]);
		link[glc] = { 0, gmc, 0,0 };

		int h = mail[gmc].val[i] % HASH; 
		while (hashhead[h].val != 0) {
			if (hashhead[h].val == mail[gmc].val[i] ) {
				break; 
			}
			else h = (h + 1) % HASH + 1; 
		}

		hashhead[h].val = mail[gmc].val[i];
		garolink(&link[glc], hashhead[h].n);
		garolink(&hashhead[h], &link[glc]);
		glc++;
	}
	
	F(i, 0, rc - 1) {
		int id = rids[i];

		user[id].list[user[id].end++] = gmc; 
		

		if (user[id].num == K) {			
			user[id].start++;
		}
		else {
			user[id].num++;
		}
		
		//log("user[%d]: num = %d  start = %d, end = %d\n", id, user[id].num ,user[id].start,  user[id].end);
	}
	gmc++;
	log("----\n");
}

int getCount(int uid)
{
	log("++++ uid = %d \n", uid);
	int ret = user[uid].num;


	log("---- ret = %d\n", ret);
	return ret;
}

int deleteMail(int uid, char str[])
{
	log("++++ uid = %d, str = %s \n", uid, str);
	log("num = %d  start = %d, end = %d\n", user[uid].num, user[uid].start, user[uid].end);

	Mail temp = { 0, };
	getlong(str, &temp);
	int ret = 0;

	F(i, user[uid].start, user[uid].end-1) {
		if (user[uid].list[i] != -1 && temp == mail[user[uid].list[i]]) {
			user[uid].list[i] = -1;
			user[uid].num--; 
			ret++;
			//log("user[%d]: num = %d  start = %d, end = %d\n", uid, user[uid].num, user[uid].start, user[uid].end);
		}
		else continue;
	}

	log("----ret = %d\n", ret );
	return ret;
}

int searchMail(int uid, char t[])
{
	log("++++uid = %d , str = %s \n", uid, t);
	log("num = %d  start = %d, end = %d\n", user[uid].num, user[uid].start, user[uid].end);  
	int ret = 0;
	Mail temp = { 0, };
	getlong(t, &temp);

	int h = temp.val[0] % HASH; 

	while (hashhead[h].val != temp.val[0]) {
		if (hashhead[h].val == 0) return ret; 
		h = (h + 1) % HASH + 1;
	}

	Link * s = hashhead[h].n;
	bool check[MAX + 1] = { 0, };
	while (s != &hashtail[h]) {
		if (check[s->mid]) ; 
		else {
			check[s->mid] = true;	
		}
		s = s->n;
	}

	F(i, user[uid].start, user[uid].end - 1) {

		//log("list = %d", user[uid].list[i]);
		if (user[uid].list[i] == -1) continue; 
		if (check[user[uid].list[i]] ) ret++;
	}
	log("----ret = %d\n", ret);
	return ret;
}

