
#define DDD 0
 
#if DDD 
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a, __FUNCTION__, __LINE__,##__VA_ARGS__ )
#define logn(a,...)	printf("%10s[%03d]: " a "\n", __FUNCTION__, __LINE__,##__VA_ARGS__ )

#else
#define log(a,...)
#define logn(a,...)	
#endif 

template <typename T>
void Swap(T&a, T&b) {
	T t = a; a = b; b = t; 
}

#define F(a,b,c)	for ( register int a=b; a<=c; a++)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

#define MU    1000
#define MT    5000
#define MM	50000
#define ML		(MM*300 )

// The below commented functions are for your reference. If you want 
// to use it, uncomment these functions.

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

#define MAX_KEY 10
#define MAX_DATA 128
#define MAX_TABLE (MT*2 +7)
#define NFD -9999999

typedef struct
{
	char key[MAX_KEY + 1];
}Hash;
Hash tb[MAX_TABLE];

unsigned long _hash(const char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
	{
		hash = (((hash << 5) + hash) + c) % MAX_TABLE;
	}

	return hash % MAX_TABLE;
}

int find(const char *key)
{
	unsigned long h = _hash(key);
	int cnt = MAX_TABLE;

	while (tb[h].key[0] != 0 && cnt--)
	{
		if (mstrcmp(tb[h].key, key) == 0)
		{
			return h;
		}
		h = (h + 1) % MAX_TABLE;
	}
	return NFD;
}

int add(const char *key)
{
	unsigned long h = _hash(key);

	while (tb[h].key[0] != 0)
	{
		if (mstrcmp(tb[h].key, key) == 0)
		{
			return h;
		}

		h = (h + 1) % MAX_TABLE;
	}
	mstrcpy(tb[h].key, key);
	return h;
}


class Msg {
public: 
	int mid; 
	int uid; 
	int t;
	friend int operator <  (Msg & l , Msg & r) {
		if (l.t == r.t) return l.uid < r.uid; 
		else return l.t > r.t; 
	}
};

Msg msg[MM + 1]; 
int gmc = 0; 

Msg * newMsg(int mid, int t, int uid){
	Msg * mp = &msg[gmc++]; 
	mp->mid = mid; 
	mp->t = t; 
	mp->uid = uid; 

	return mp; 
}

class Link {
public: 
	Msg * mp; 
	Link *n; 
	Link *p; 
};
Link link[ML + 1]; 
int glc = 0; 

Link * newLink(Msg * mp) {
	Link * lp = &link[glc++]; 
	lp->mp = mp; 
	lp->n = lp->p = 0; 
	return lp;
}


Link * user[MU + 1]; 
Link * hash[MAX_TABLE]; 

int umap[MU + 1][MU + 1]; 

void init()
{
	F(i, 0, MU) user[i] = 0; 
	F(i, 0, MAX_TABLE - 1)	 hash[i] = 0, tb[i].key[0] = 0;


	F(i, 0, MU) {
		F(j, 0, MU) umap[i][j] = 0;
		umap[i][0] = 1, umap[i][1] = i; 
	}
	glc = gmc = 0; 
}

void createMessage(int mid, int uid, char msgs[])
{
	log("++++ mid = %d, uid = %d, msg = %s \n", mid, uid, msgs); 

	int t = ((msgs[0] - '0') * 10 + (msgs[1] - '0')) * 3600 + ((msgs[3] - '0') * 10 + (msgs[4] - '0')) * 60 + ((msgs[6] - '0') * 10 + (msgs[7] - '0')); 

	logn("t = %d", t); 
	Msg * mp = newMsg(mid, t, uid); 

	Link * lp = newLink(mp); 

	lp->n = user[uid]; 
	if (user[uid]) user[uid]->p = lp; 
	user[uid] = lp; 

#if DDD
	log("uid %d : ",uid); 
	while (lp) {
		printf("%d -> ", lp->mp->mid); 
		lp = lp->n; 
	}
	printf("\n");
#endif 

	char *s, *e; 
	char c; 
	s = e = &msgs[8]; 

	while (1) {
		if (*e == ' ' || *e == 0)
		{
			c = *e;
			*e = 0; 

			int h = add(s);
			lp = newLink(mp);

			lp->n = hash[h];
			if (hash[h]) hash[h]->p = lp;
			hash[h] = lp;
#if DDD

			log("(%s) h %d : ", s, h);
			while (lp) {
				printf("%d -> ", lp->mp->mid);
				lp = lp->n;
			}
			printf("\n");
#endif 
			*e = c;
			if (*e == 0) break;
			s = ++e;
		}
		else e++; 
	}	

	log("---- \n\n"); 
}

void followUser(int id1, int id2)
{
	log("++++ id1 = %d, id2= %d \n ", id1, id2	);

	int cnt = ++umap[id1][0]; 

	umap[id1][cnt] = id2; 

#if DDD
	log("F %d , cnt = %d : ", id1, cnt);
	F(i,1,cnt) {
		printf("%d -> ", umap[id1][i]);		
	}
	printf("\n");
#endif 

	log("---- \n\n");
}

#define MR 5

class Ret {
public:
	int cnt; 
	Msg  list[MR + 1]; 

	void add(Msg mp) {
		list[cnt++] = mp; 

		for (register int  i = cnt -1; i > 0; i--)
		{
			if (list[i] < list[i-1])
			{
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

int searchByHashtag(char name[], int rets[])
{
	Ret ret = { 0, }; 

	int h = find(name); 
	if (h == NFD) return ret.cnt; 

	Link * lp = hash[h]; 

	while (lp)
	{
		Msg  * mp = lp->mp; 
		ret.add(*mp); 
		lp = lp->n; 
	}

	F(i, 0, ret.cnt - 1)rets[i] = ret.list[i].mid;
	return ret.cnt; 
}

int getMessages(int uid, int rets[]){

	log("++++ uid = %d\n ", uid);
	Ret ret = { 0, };

	int fcnt = umap[uid][0]; 

	F(i, 1, fcnt) {
		int fid = umap[uid][i]; 

		Link * lp = user[fid]; 

		while (lp)
		{
			Msg  * mp = lp->mp;
			ret.add(*mp);
			lp = lp->n;
		}
	}

	F(i, 0, ret.cnt - 1)rets[i] = ret.list[i].mid; 	
	log("---- \n\n");
	return ret.cnt;
}

