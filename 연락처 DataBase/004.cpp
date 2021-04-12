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

#define DDD 0

#if DDD 
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]:" a "\n",__FUNCTION__, __LINE__,##__VA_ARGS__)	
#else 
#define log(a,...)
#endif 

#define F(a,b,c)	for(register int a=b; a<=c; a++)
#define NF(a,b,c)	for(register int a=b; a>=c; a--)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

template <typename T>
void Swap(T &a, T &b) {
	T t = a;
	a = b;
	b = t; 
}




#define MF	5  // 5개 필드 
#define MSTR	19  // 항목당 최대 19글자 
#define MR 50000   // 총 5만개 항목 

#define MAX_KEY MSTR
#define MAX_TABLE (MR*2+7)

typedef struct
{
	char key[MAX_KEY + 1];
}_Hash;
_Hash tb[MF][MAX_TABLE];

#define NFND -9999999

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

int find(const char *key,int f)
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
	return NFND;
}

int add(const char *key, int f)
{
	unsigned long h = _hash(key);
	int k = 0; 

	while (tb[f][h].key[0] != 0)
	{
		if (mstrcmp(tb[f][h].key, key) == 0)
		{
			return h;
		}

		h = (h + 1) % MAX_TABLE;
		k++; 
	}
	mstrcpy(tb[f][h].key, key);
	return h;
}

int grc = 0; 
class Rec {
public: 
	char str[MF][MSTR + 1]; 
	void init() { F(i, 0, MEMO) str[i][0] = 0;  }
}rec[MR+1];
Rec * newRec() {
	Rec * rp = &rec[grc++]; 
	F(i, 0, MEMO) rp->str[i][0] = 0;
	return rp; 
}

int glc = 0; 
#define ML (MR*2)
class Link {
public: 
	Rec * rp; 
	Link *n[MF]; 
	Link *p[MF];
}link [ML+1];
Link * newLink(Rec * rp) {
	Link * lp = &link[glc++];
	lp->rp = rp; 
	F(i, 0, MEMO) lp->n[i] = lp->p[i] = 0;
	return lp; 
}

class Hash {
public: 
	int cnt;
	Link head; 
	Link tail;
	void init() {
		F(i, 0, MEMO) {
			head.n[i] = &tail;
			tail.p[i] = &head;
			head.p[i] = tail.n[i] = 0; 
		}
		cnt = 0; 
	}
}hash[MF][MAX_TABLE];


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void InitDB()
{
	log("++++");
	F(i, 0, MEMO)F(j, 0, MAX_TABLE - 1) hash[i][j].init(), tb[i][j].key[0] = 0; 
	glc = grc = 0;
	log("----\n");
}

void Add(char* name, char* num, char* day, char* mail, char* memo)
{
	log("++++");
	log("name : %s", name); 
	log("num : %s", num);
	log("day : %s", day);
	log("mail : %s", mail);
	log("memo : %s", memo);

	Rec * rp = newRec(); 
	mstrcpy(rp->str[NAME], name); 
	mstrcpy(rp->str[NUM], num);
	mstrcpy(rp->str[DAY], day);
	mstrcpy(rp->str[MAIL], mail);
	mstrcpy(rp->str[MEMO], memo);

	Link * lp = newLink(rp); 

	F(i, 0, MEMO) {
		int h = add(rp->str[i], i); 

		lp->n[i] = hash[i][h].head.n[i]; 
		lp->p[i] = &hash[i][h].head;   //  head 가 앞에 하나 더 있는 것이니 

		if (hash[i][h].head.n[i])hash[i][h].head.n[i]->p[i] = lp; 
		hash[i][h].head.n[i] = lp; 
		hash[i][h].cnt++; 
		log("str = %s h= %d, cnt = %d",rp->str[i], h, hash[i][h].cnt); 
	}


	log("----\n");
}

int Delete(FIELD f, char* str)
{
	log("++++ f %d, str=%s",f, str);
	int ret = 0; 

	int h = find(str, f);
	if (h == NFND) return ret; 

	Link * lp = hash[f][h].head.n[f]; 

	log("h=%d, cnt = %d", h, hash[f][h].cnt); 

	while (lp != &hash[f][h].tail) {
		ret++; 
		Link * next = lp->n[f];
		hash[f][h].cnt--; 
		F(i, 0, MEMO) {
			if (lp->n[i]) lp->n[i]->p[i] = lp->p[i];
			if (lp->p[i]) lp->p[i]->n[i] = lp->n[i];
		}
		lp = next;
	}
	log("h=%d, cnt = %d", h, hash[f][h].cnt);

	log("----ret = %d	\n",ret); 	
	return ret;
}

int Change(FIELD f, char* str, FIELD cf, char* cstr)
{
	log("++++ f %d, str=%s  cf=%d, cstr = %s",f, str, cf, cstr);
	int ret = 0;
	int h = find(str, f);
	if (h == NFND) return ret;

	Link * lp = hash[f][h].head.n[f];

	log("h=%d, cnt = %d", h, hash[f][h].cnt);

	while (lp != &hash[f][h].tail) {
		ret++;
		Link * next = lp->n[f]; 
		
		if (lp->n[cf]) lp->n[cf]->p[cf] = lp->p[cf];
		if (lp->p[cf]) lp->p[cf]->n[cf] = lp->n[cf];

		mstrcpy(lp->rp->str[cf], cstr); 
		int ch = add(cstr, cf); 

		lp->n[cf] = hash[cf][ch].head.n[cf];
		lp->p[cf] = &hash[cf][ch].head;   //  head 가 앞에 하나 더 있는 것이니 

		if (hash[cf][ch].head.n[cf])hash[cf][ch].head.n[cf]->p[cf] = lp;
		hash[cf][ch].head.n[cf] = lp;

		
		lp = next;
	}
	

	log("---- ret= %d\n",ret	);
	
	return ret;
}

RESULT Search(FIELD f, char* str, FIELD rf)
{
	log("++++ %d , %s %d",f, str , rf);
	RESULT ret = { 0, };

	int h = find(str, f);
	if (h == NFND) return ret;

	Link * lp = hash[f][h].head.n[f];

	while (lp != &hash[f][h].tail)
	{
		mstrcpy(ret.str, lp->rp->str[rf]); 
		ret.count++; 

		lp = lp->n[f];
	}
	log("---- ret= %d\n", ret.count);
	return ret;
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