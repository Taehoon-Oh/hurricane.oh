template <typename T>
void Swap(T&a, T&b) {
	T t = a; a = b; b = t;
}
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))


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

#define MAX 50000
#define HASH (MAX*2+7) 
#define USER 1000

#define MAX_KEY 10
//#define MAX_DATA 128
#define MAX_TABLE HASH

typedef struct
{
	char key[MAX_KEY + 1];
	//char data[MAX_DATA + 1];
}Hash;
Hash tb[MAX_TABLE];

//�Ʒ��� �⺻�ε� ���� ������ ���� ����� ������ ������ �ȴ�. 
unsigned long hash(const char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
	{
		hash = (((hash << 5) + hash) + c) % MAX_TABLE;
	}

	return hash % MAX_TABLE;
}

//int find(const char *key, char *data)
int find(const char *key)
{
	unsigned long h = hash(key);
	int cnt = MAX_TABLE;

	while (tb[h].key[0] != 0 && cnt--)
	{
		if (mstrcmp(tb[h].key, key) == 0)
		{
			//strcpy(data, tb[h].data);
			//return 1;
			return h;
		}
		h = (h + 1) % MAX_TABLE;
	}
	//return 0;
	return -1;  // ��ã�������� ���� �ʿ��ϴ�. 
}


//int add(const char *key, char *data)
int add(const char *key)
{
	unsigned long h = hash(key);

	while (tb[h].key[0] != 0)
	{
		if (mstrcmp(tb[h].key, key) == 0)
		{
			return h;
		}

		h = (h + 1) % MAX_TABLE;
	}
	mstrcpy(tb[h].key, key);
	//strcpy(tb[h].data, data);
	//return 1;
	return h;
}


class Msg {
public:
	int mid;
	int uid;
	int t;	
	// �Ʒ��� ��ũ�帮��Ʈ���� ���� �� �Ҷ� �Ʒ� ó�� �� ���� �ִ�. 
	/*
	bool operator <  ( Msg right) {
		if (t == right.t) return uid < right.uid;  //
		else return t > right.t;
	}
	*/

	//���� �ֱٿ� ������ �޽����� �켱�� �ǰ�, �����ð��̰��� ��� ���� id �� �������� �켱�� �ȴ�.
	// user id �� �� 1000 ���� ���� 1000 �뷫  2^10  
	// score = time <<10  - uid  �� , time �� <<10  (*1024) �� �ؼ� �����ϸ� �ð� ���� ���� ��쿡�� uid �� ������ �ް� �ȴ�. 
	int score; 

};

Msg msg[MAX + 1];
int gmc = 0;

Msg * newMsg(int mid, int t, int uid) {
	Msg * mp = &msg[gmc++];
	mp->mid = mid;
	mp->t = t;
	mp->uid = uid;
	//���� �ֱٿ� ������ �޽����� �켱�� �ǰ�, �����ð��̰��� ��� ���� id �� �������� �켱�� �ȴ�.
	// user id �� �� 1000 ���� ���� 1000 �뷫  2^10  
	// score = time <<10  - uid  �� , time �� <<10  (*1024) �� �ؼ� �����ϸ� �ð� ���� ���� ��쿡�� uid �� ������ �ް� �ȴ�. 	
	mp->score = (t << 10) - uid; // (t<<10) ��ȣ�� �� �ؾ� �Ѵ� 

	return mp;
}

class Link {
public:
	Msg * mp;
	Link *n;
	Link *p; // ���߿� prev �� ���� ������ ���´�. 
};
Link link[(MAX * 12)];
int glc = 0;

Link * newLink(Msg * mp) {
	Link * lp = &link[glc++];
	lp->mp = mp;
	lp->n = lp->p = 0; // ���߿� prev �� ���� ������ ���´�. 
	return lp;
}

#define garolink(left,right) (left)->n=right; (right)->p=left
//���߿� ->p (������ũ)�� ���� ���ٸ� �Ʒ���  �ٲ۴�. 
// #define garolink(left,right) (left)->n=right

Link userhead[USER + 1], usertail[USER + 1];
Link hashhead[HASH], hashtail[HASH];

int umap[USER + 1][USER + 1];

void init()
{
	for (register int i = 0; i <= USER; i++) {
		garolink(&userhead[i], &usertail[i]); 
	}
	for (register int i = 0; i <= HASH - 1; i++) {
		garolink(&hashhead[i], &hashtail[i]);
	}

	for (register int i = 0; i <= USER; i++) {
		for (register int j = 0; j <= USER; j++) umap[i][j] = 0;
		umap[i][0] = 1, umap[i][1] = i;
	}
	glc = gmc = 0;
}

void createMessage(int mid, int uid, char msgs[])
{
	int t = ((msgs[0] - '0') * 10 + (msgs[1] - '0')) * 3600 + ((msgs[3] - '0') * 10 + (msgs[4] - '0')) * 60 + ((msgs[6] - '0') * 10 + (msgs[7] - '0'));
	//���� �ֱٿ� ������ �޽����� �켱�� �ǰ�, �����ð��̰��� ��� ���� id �� �������� �켱�� �ȴ�.
	// user id �� �� 1000 ���� ���� 1000 �뷫  2^10  
	// score = time <<10  - uid  �� , time �� <<10  (*1024) �� �ؼ� �����ϸ� �ð� ���� ���� ��쿡�� uid �� ������ �ް� �ȴ�. 	
	Msg * mp = newMsg(mid, t, uid);
	Link * lp = newLink(mp);

	garolink(lp, userhead[uid].n); 
	garolink(&userhead[uid], lp );

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

			garolink(lp, hashhead[h].n);
			garolink(&hashhead[h], lp);

			*e = c;
			if (*e == 0) break;
			s = ++e;
		}
		else e++;
	}
}

void followUser(int id1, int id2)
{
	int cnt = ++umap[id1][0];
	umap[id1][cnt] = id2;
}

//�ְ� 5���� ���Ҷ� ���. 
//�̷��� ���� �ʰ� ó������ ��ũ�� ����Ʈ�� ������� ������ ���� �ִ�. 
class Ret {
public:
	int cnt;
	Msg*  list[5 + 1];

	void add(Msg *mp) {
		list[cnt++] = mp;

		for (register int i = cnt - 1; i > 0; i--)
		{
			//if (list[i]e  < list[i - 1])  // Score �� ���� �ʰ� ������� �Ѵٸ�.. Msg class ���� class �� �� �� �� �ֵ���  operator overriding �� �ؼ� ����� �����ϴ�. 
			if (list[i]->score > list[i - 1]->score)
			{
				Swap(list[i], list[i - 1]);
			}
			else
			{
				break;
			}
		}

		cnt = Min(cnt,5);
	}

};

int searchByHashtag(char name[], int rets[])
{
	Ret ret = { 0, };

	int h = find(name);
	if (h == -1) return ret.cnt;

	Link * lp = hashhead[h].n;

	while (lp!= &hashtail[h])
	{
		Msg  * mp = lp->mp;
		ret.add(mp);
		lp = lp->n;
	}

	for (register int i = 0; i <= ret.cnt - 1; i++)rets[i] = ret.list[i]->mid;
	return ret.cnt;
}

int getMessages(int uid, int rets[]) {
	Ret ret = { 0, };

	int fcnt = umap[uid][0];

	for (register int i = 1; i <= fcnt; i++) {
		int fid = umap[uid][i];

		Link * lp = userhead[fid].n;

		while (lp != &usertail[fid])
		{
			Msg  * mp = lp->mp;
			ret.add(mp);
			lp = lp->n;
		}
	}

	for (register int i = 0; i <= ret.cnt - 1; i++)rets[i] = ret.list[i]->mid;
	return ret.cnt;
}
