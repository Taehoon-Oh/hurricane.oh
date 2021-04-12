#define EE 0

#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]:" a "\n",__FUNCTION__,__LINE__,##__VA_ARGS__)

#else
#define log(a,...)

#endif 
#define Max(a,b) (((a)>(b))?(a):(b))
#define Min(a,b) (((a)<(b))?(a):(b))

#define F(a,b,c)	for (register int a=b;a<=c;a++)
#define NF(a,b,c)	for (register int a=b;a>=c;a--)

template <typename T>
void Swap(T& a, T&b) { T t = a; a = b; b = t; }

#define MSTR 19
#define MR 50000
#define MAX_KEY MSTR
#define MAX_TABLE (MR*2 +7)
#define ML MAX_TABLE
#define MF 5
#define NFD -9999999
typedef struct
{
	char key[MAX_KEY + 1];
}_Hash;
_Hash tb[MF][MAX_TABLE];

int add(const char *key, int f);
int find(const char *key, int f);
int mstrcmp(const char *a, const char *b);
void mstrcpy(char *dest, const char *src);

typedef enum
{
	NAME,
	NUM,
	DAY,
	MAIL,
	MEMO
} FIELD;

typedef struct
{
	int count;
	char str[20];
} RESULT;



int grc = 0;
typedef struct _Rec {
	char str[MF][MSTR + 1];
}Rec;
Rec rec[MR + 1];

Rec * newRec() {
	Rec* rp = &rec[grc++]; 	return rp; 
}

int glc = 0; 
typedef struct _Link {
	Rec * rp; 
	_Link *n[MF];
	_Link * p[MF];
}Link;
Link link[ML + 1];

Link * newLink(Rec * rp) {
	Link * lp = &link[glc++];
	lp->rp = rp;
	return lp;
}

struct Hash {
	int cnt;
	Link head;
	Link tail;
	//void init(){
	//	cnt = 0;
	//	F(i, 0, MEMO) {
	//		head.n[i] = &tail, tail.p[i] = &head;
	//		tail.n[i] = head.p[i] = 0;
	//	}
	//}

}hash[MF][MAX_TABLE];

void InitDB()
{
	log("++++"); 
	F(i, 0, grc - 1) rec[i] = { 0, };
	F(i, 0, glc - 1) link[i] = {0, };
	F(j, 0, MEMO)F(i, 0, MAX_TABLE - 1) {
		hash[j][i].head.n[j] = &hash[j][i].tail, hash[j][i].tail.p[i] = &hash[j][i].head, tb[j][i].key[0] = 0;
	}
	grc = glc = 0; 
	log("----\n");
}

void Add(char* name, char* num, char* day, char* mail, char* memo)
{
	log("++++ name=%s, num=%s, day = %s, mail = %s, memo = %s", name, num, day, mail, memo);

	Rec * rp = newRec();
	mstrcpy(rp->str[NAME], name);
	mstrcpy(rp->str[NUM], num);
	mstrcpy(rp->str[DAY], day);
	mstrcpy(rp->str[MAIL], mail);
	mstrcpy(rp->str[MEMO], memo);

	Link * lp = newLink(rp); 
	F(i, 0, MEMO) {
		int h = add(rp->str[i], i); 
		log("h=%d", h); 
		lp->n[i] = hash[i][h].head.n[i];
		lp->p[i] = &hash[i][h].head;

		if (hash[i][h].head.n[i]) hash[i][h].head.n[i]->p[i] = lp; 
		hash[i][h].head.n[i] = lp; 

		hash[i][h].cnt++; 
		/*if (hash[i][h].cnt == 2) {
			lp = hash[i][h].head.n[i];

			if (lp->n[i]) lp->n[i]->p[i] = lp->p[i];
			if (lp->p[i]) lp->p[i]->n[i] = lp->n[i];
			hash[i][h].cnt--;

			if (hash[i][h].cnt == 1) {
				lp = hash[i][h].head.n[i];
				if (lp->n[i]) lp->n[i]->p[i] = lp->p[i];
				if (lp->p[i]) lp->p[i]->n[i] = lp->n[i];
				hash[i][h].cnt--;

				if (hash[i][h].cnt == 0) {
					lp = hash[i][h].head.n[i];
					if (lp->n[i]) lp->n[i]->p[i] = lp->p[i];
					if (lp->p[i]) lp->p[i]->n[i] = lp->n[i];

				}
			}
		}*/
	}

	log("----\n");
}

int Delete(FIELD f, char* str)
{
	int ret = 0;
	log("++++ f=%d, str = %s",f, str);
	int h = find(str, f);
	if (h == NFD) return ret; 

	log("h=%d, str=%s", h, tb[f][h].key); 

	Link * lp = hash[f][h].head.n[f];

	while (lp != &hash[f][h].tail) {
		ret++; 		
		F(i, 0, MEMO) {
			if (lp->p[i])lp->n[i]->p[i] = lp->p[i];
			if (lp->n[i])lp->p[i]->n[i] = lp->n[i];
		}
		hash[f][h].cnt--;
		lp = lp->n[f];
	}

	log("----ret = %d\n", ret);
	return ret;
}

int Change(FIELD f, char* str, FIELD cf, char* cstr)
{
	int ret = 0;
	log("++++ f=%d, str = %s , cf=%d, cstr= %s", f, str, cf, cstr);

	int h = find(str, f);
	if (h == NFD) return ret;

	log("h=%d, str=%s", h, tb[f][h].key);

	Link * lp = hash[f][h].head.n[f];

	while (lp != &hash[f][h].tail) {
		
		// 꼭 next 를 미리 값을 chec 해야 한다 아래 지우거나 삭제 하면서 바뀔수 있으니까. 
		Link * next = lp->n[f];



		// 일단 지운다. 
		if (lp->n[cf]) lp->n[cf]->p[cf] = lp->p[cf];
		if (lp->p[cf]) lp->p[cf]->n[cf] = lp->n[cf];

		mstrcpy(lp->rp->str[cf], cstr); 

		int k  = add(cstr, cf);
		lp->n[cf] = hash[cf][k].head.n[cf];
		lp->p[cf] = &hash[cf][k].head;

		if ( hash[cf][k].head.n[cf]) hash[cf][k].head.n[cf]->p[cf] = lp;
		hash[cf][k].head.n[cf] = lp;

		lp= next;
		ret++; 
	}
	log("----ret = %d\n",ret);
	return ret;
}

RESULT Search(FIELD f, char* str, FIELD rf)
{
	RESULT ret = { 0, };
	log("++++ f=%d, str = %s rf=%d", f, str, rf);

	int h = find(str, f);
	if (h == NFD) return ret;

	log("h=%d, str=%s", h, tb[f][h].key);

	Link * lp = hash[f][h].head.n[f];

	while (lp != &hash[f][h].tail)
	{
		ret.count++;
		mstrcpy(ret.str, lp->rp->str[rf]); 
		log("cnt =%d, str = %s\n", ret.count, ret.str);
		lp = lp->n[f]; 
	}

	log("----cnt =%d, str = %s\n", ret.count, ret.str);
	return ret;
}

















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

int find(const char *key, int f)
{
	unsigned long h = _hash(key);
	int cnt = MAX_TABLE;

	while (tb[f][h].key[0] != 0 && cnt--)
	{
		if (mstrcmp(tb[f][h].key, key) == 0)
		{
			return h;
		}
		h = (h + 1) % MAX_TABLE;
	}
	return NFD;
}

int add(const char *key, int f)
{
	unsigned long h = _hash(key);

	while (tb[f][h].key[0] != 0)
	{
		if (mstrcmp(tb[f][h].key, key) == 0)
		{
			return h;
		}

		h = (h + 1) % MAX_TABLE;
	}
	mstrcpy(tb[f][h].key, key);
	return h;
}

void mstrcpy(char *dest, const char *src)
{
	int i = 0;
	while (src[i] != 0) { dest[i] = src[i]; i++; }
	dest[i] = src[i];
}

int mstrcmp(const char *a, const char *b)
{
	int i;
	for (i = 0; a[i] != 0; i++) { if (a[i] != b[i]) return a[i] - b[i]; }
	return a[i] - b[i];
}
