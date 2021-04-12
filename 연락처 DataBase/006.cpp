#define EE 1

#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define log(a,...)
#endif


template <typename T >
void swap(T &a, T&b) { T t = a; a = b; b = t; }
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

#define F(a,b,c)	for(register int a =b;a<=c;a++)
#define NF(a,b,c)	for(register int a =b;a>=c;a--)


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

#define MF 5
#define Mstr 19
#define R 50000
#define MAX_TABLE (R*2 +7)

int gr = 0;
class Rec{
public:
	int rid; 
	char str[MF][Mstr + 1];
	Rec * n[MF];
	Rec * p[MF]; 
}rec[R+1];
Rec * newRec() {
	Rec * rp = &rec[gr++]; 
	rp->rid = gr - 1;
	return rp;
}

class hash {
public:
	int h;
	Rec tail;
	Rec head;
}hash[MAX_TABLE];



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

Rec * find(const char *key, int f)
{
	unsigned long h = _hash(key);
	int cnt = MAX_TABLE;

	Rec * rp = hash[h].head.n[f];
	Rec * tail = &hash[h].tail;

	while (rp != tail && rp[h].str[f][0] != 0 && cnt--)
	{
		if (mstrcmp(rp[h].str[f], key) == 0)
		{
			return rp;
		}
		h = (h + 1) % MAX_TABLE;
		rp = hash[h].head.n[f];
	}
	return 0;
}

int add(const char *key, int f)
{
	unsigned long h = _hash(key);

	Rec * rp = hash[h].head.n[f];

	while ( rp->str[f][0] != 0)
	{
		if (mstrcmp(rp[h].str[f], key) == 0)
		{
			return h;
		}

		h = (h + 1) % MAX_TABLE;
		rp = hash[h].head.n[f];
	}
	return h;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void InitDB()
{
	gr = 0;
	F(i, 0, MAX_TABLE - 1) {
		hash[i].h = 0;
		F(f, 0, MEMO) {
			hash[i].head.n[f] = &hash[i].tail;
			hash[i].tail.p[f] = &hash[i].head;
			hash[i].head.p[f] = hash[i].tail.n[f] = 0;
		}
	}
}

void Add(char* name, char* num, char* day, char* mail, char* memo)
{
	log("++++");

	Rec * rp = newRec();
	mstrcpy(rp->str[NAME], name);
	mstrcpy(rp->str[NUM], num);
	mstrcpy(rp->str[DAY], day);
	mstrcpy(rp->str[MAIL], mail);
	mstrcpy(rp->str[MEMO], memo);

	F(f, 0, MEMO) {
		int h   = add(rp->str[f], f);

		rp->n[f] = hash[h].head.n[f]; 
		rp->p[f] = &hash[h].tail; 
		if (hash[h].head.n[f]) hash[h].head.p[f] = rp; 
		hash[h].head.n[f] = rp; 
	}

	log("----");
}

int Delete(FIELD f, char* str)
{
	log("++++");
	int ret = 0;

	Rec * rp = find(str, f); 	
	if (rp == 0)
	{
		return ret;
	}
	
	while (rp->n[f] !=0 )
	{
		F(i, 0, MEMO) {
			if (rp->n[i]) rp->n[i]->p[i] = rp->p[i];
			if (rp->p[i]) rp->p[i]->n[i] = rp->n[i];
		}
		rp = rp->n[f]; 
	}

	log("----");
	return -1;
}

int Change(FIELD field, char* str, FIELD changefield, char* changestr)
{
	log("++++");

	log("----");
	return -1;
}

RESULT Search(FIELD f, char* str, FIELD rf)
{
	RESULT result;
	result.count = 0;
	log("++++");

	Rec * rp = find(str, f);
	if (rp == 0)
	{
		return result;
	}

	while (rp->n[f] != 0)
	{
		mstrcpy(result.str, rp->str[rf]);
		result.count++; 
		rp = rp->n[f];
	}


	log("----");

	return result;
}
