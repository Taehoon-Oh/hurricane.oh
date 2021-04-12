
#define EE 1
#include <queue>
#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]:" a "\n", __FUNCTION__,__LINE__,##__VA_ARGS__)
#else
#define log(a,...)
#endif


template <typename T>
void swap(T&a, T&b) { T t = a; a = b; b = t; }
#define F(a,b,c)	for(register int a=b;a<=c;a++)
#define NF(a,b,c)	for(register int a=b;a>=c;a--)

#define MAX_USER      1000
#define MAX_TAG       5000

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
typedef unsigned long long ll;

#define Mhash 5000
#define MAX_TABLE ( Mhash*2 + 7)
typedef struct
{
	ll key;

}_Hash;
_Hash tb[MAX_TABLE];

ll _hash(const char *str)
{
	ll hash = 0;


	while (*str != '\0')
	{
		hash = (hash << 5) + *str - 'a' + 1;
		str++;
	}

	return hash;
}
#define NFD -9999999
int find(ll key)
{
	int h = key % MAX_TABLE;
	int cnt = MAX_TABLE;

	while (tb[h].key != 0 && cnt--)
	{
		if (tb[h].key == key)
		{
			return h;
		}
		h = (h + 1) % MAX_TABLE;
	}
	return NFD;
}

int add(ll key)
{
	int h = key % MAX_TABLE;
	while (tb[h].key != 0)
	{
		if (tb[h].key == key)
		{
			return h;
		}
		h = (h + 1) % MAX_TABLE;
	}
	tb[h].key = key;
	return h;
}

#define MM 50000
int gm = 0;
class Msg {
public:
	int mid;
	int time;
	int uid;


	//bool operator > (const rect& rhs) const { return _w * _h > rhs._w * rhs._h; }
	bool operator < ( const Msg &pre) const {
		if (time == pre.time)
		{
			return uid < pre.uid;
		}
		else
		{
			return time > pre.time;
		}
	}
};



#define MU 1000
std::priority_queue <Msg *, std::vector<Msg>, std::less<Msg> >  user[MU + 1];

#define ML (MM*10 +MU*100)
int gl = 0;
class Link {
public:
	Msg * mp;
	Link *n;
	Link *p;
}link[ML + 1];

Link * newLink(Msg * mp) {
	Link * lp = &link[gl++];
	lp->mp = mp;
	return lp;
}

//class User {
//public:
//	int num;
//	Link head;
//	Link tail;
//}user[MU + 1];


class Hash {
public:
	int num;
	Link head;
	Link tail;

}hash[MAX_TABLE];

int fmap[MU + 1][MU + 1];





void init()
{
	_hash("lotxhub");
	_hash("hotxhub");
	//F(i, 0, gm - 1)msg[i] = { 0, };
	gm = 0;
	F(i, 0, gl - 1) link[i] = { 0, };
	gl = 0;

	F(i, 0, MU)while (!user[i].empty()) user[i].pop();

	F(i, 0, MAX_TABLE - 1)hash[i].num = 0, hash[i].head.n = &hash[i].tail, hash[i].tail.p = &hash[i].head, tb[i].key = 0;

	F(i, 0, MU) {
		F(j, 0, fmap[i][0])  fmap[i][j] = 0;
		fmap[i][0] = 1, fmap[i][1] = i;
	}

}

void createMessage(int mid, int uid, char msg[])
{
	log("++++ mid = %d, uid = %d, msg= %s", mid, uid, msg);
	Msg temp; 
	Msg * mp = &temp;
	mp->mid = mid;
	mp->uid = uid;
	mp->time = msg[0] * 3600 * 10 + msg[1] * 3600 + msg[3] * 60 * 10 + msg[4] * 60 + msg[6] * 10 + msg[7];

	char * s, *e, c;
	s = e = msg + 10;
	while (1) {
		if (*e == ' ' || *e == '\0') // 해쉬 계산한다. 
		{
			c = *e;
			*e = 0;

			ll temp = _hash(s);
			int h = add(temp);
			Link * lp = newLink(mp);
			log("link %d ", gl - 1);

			lp->n = hash[h].head.n;
			lp->p = &hash[h].head;

			if (hash[h].head.n)hash[h].head.p = lp;
			hash[h].head.n = lp;

			hash[h].num++;
			log("%s -> %lld, h=%d, hash[%d].num=%d", s, temp, h, h, hash[h].num);

			*e = c;
			if (*e == '\0')
			{
				break;
			}

			s = ++e;

		}
		else if (*e == '#')
		{
			s = ++e;
		}
		else
		{
			e++;
		}
	}

	/*Link * lp = newLink(mp);

	lp->n = user[uid].head.n;
	lp->p = &user[uid].head;

	if (user[uid].head.n) user[uid].head.p = lp;
	user[uid].head.n = lp;
	user[uid].num++;*/

	//user[uid].push(*mp); 
	log("user[%d].num=%d", uid, mp->mid);

	log("----\n");
}

void followUser(int u1, int u2)
{
	log("++++");
	int fcnt = ++fmap[u1][0];
	fmap[u1][fcnt] = u2;

	log("---- fmap[%d][0]=%d, %d added\n", u1, fcnt, u2);
}
#define Mret 5
#define Min(a,b)	(((a)<(b))?(a):(b))
class Ret {
public:
	int num;
	Msg * list[Mret + 1];
	void init() { num = 0; }
	void add(Msg * in) {
		list[num++]; 
		NF(i, num - 1, 1) {
			if (*list[i] < *list[i - 1]) swap(list[i], list[i - 1]); 
			else break; 
		}


		num = Min(Mret, num); 

		/*list[num++] = in;
		for (register int i = num - 1; i >= 1; i--) {
			if (*list[i] < *list[i - 1]) {
				swap(list[i], list[i - 1]);
			}
		}
		num = Min(Mret, num);*/
	}

}ret;

int searchByHashtag(char tag[], int ids[])
{
	log("++++%s", tag + 1);
	int cnt = 0;

	return cnt;
}

int getMessages(int uid, int ids[])
{
	log("++++ uid = %d", uid);
	int cnt = 0;
	
	return cnt;
}
