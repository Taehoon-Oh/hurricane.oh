#define EE 0

#if EE
//#include <stdio.h>
#define log(a,...) printf("%10s[%d]: " a "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define log(a,...)
#endif

#define F(a,b,c)	for(register int a=b;a<=c;a++)
//#define NF(a,b,c)	for(register int a=b;a>=c;a--)

//#define Max(a,b)	(((a)>(b))?(a):(b))
//#define Min(a,b)	(((a)<(b))?(a):(b))

//template <typename T>
//void swap(T &a, T &b) { T t = a; a = b; b = t; }

#define garolink(l,r) (l)->n=(r);
typedef unsigned long long ll;
/*
void mstrcpy(char dst[], const char src[]) {
	int c = 0;
	while ((dst[c] = src[c]) != '\0') ++c;
}

int mstrcmp(const char str1[], const char str2[]) {
	int c = 0;
	while (str1[c] != '\0' && str1[c] == str2[c]) ++c;
	return str1[c] - str2[c];
}
*/

#define TYPE_CNT	5
#define NAME_LEN	7
#define TAG_LEN		4
#define TAG 500
#define SECTION 100
#define MAX 50000
#define HASH (TAG*2 +7)
#define NAMEHASH (MAX*2 +7) 

class Book {
public:
	int bid;
	int sid;
	ll name;
	//char str[NAME_LEN + 1];
	bool deleted;
	//int tag[TYPE_CNT];

	//���⼭ book �ȿ� tag �� 5���� �� �� �ֱ� ������ 
	// �Ʒ�ó�� �Ѱ��� �ؼ��� �յڸ� ��ũ�帮��Ʈ�� �����Ҵ� ����� tag ���� ����Ǵ����� ���� ���� 
	// �׳��� �ȵȴ�. 
	//Book * n, *p; 
}book[MAX + 1];
int gbc = 0;

class Link {
public:
	Book * bp;
	int tag;
	Link * n;
	//Link * n, *p;
}link[MAX * 5 + 1];
int glc = 0;

Link hashhead[HASH];
Link hashtail[HASH];

// �̸����� book �� ã�ƾ� �Ҷ��� ������ �̰ɷε� �ؽ��� ����� 
// �� �̰� ����Ʈ�� ������ �ʿ� ���� ( �̸��� unique �ϴٰ� �����Ǿ� �����ϱ� 
int nametoindex[NAMEHASH];


ll getname(char str[]) {
	ll ret = 0;
	char * c = str;

	while (*c != '\0') {
		ret = (ret << 6) + *c - 'A' + 1;
		c++;
	}
	return ret;
}

int gettag(char str[]) {
	int ret = 0;
	char * c = str;

	while (*c != '\0') {
		ret = (ret << 6) + *c - 'A' + 1;
		c++;
	}
	return ret;
}

//void mstrcpy(char dst[], const char src[]) {
//	int c = 0;
//	while ((dst[c] = src[c]) != '\0') ++c;
//}

//int mstrcmp(const char str1[], const char str2[]) {
//	int c = 0;
//	while (str1[c] != '\0' && str1[c] == str2[c]) ++c;
//	return str1[c] - str2[c];
//}

int M;
void init(int m)
{
	M = m;
	log("++++ M=%d ", M);

	//���� ����ü ���ؽ� �ʱ�ȭ 
	gbc = 0;
	glc = 0;

	//hash �� ��ũ�� ����Ʈ �ʱ�ȭ 
	F(i, 0, HASH - 1) {
		hashhead[i].n = &hashtail[i];
		//hashtail[i].p = &hashhead[i];
		hashhead[i].tag = 0;
	}

	F(i, 0, NAMEHASH - 1) {
		//book �� 0 �� �ε������� ���۵Ǵ� �ʱ� ���� -1 ���� �����Ѵ�. 
		nametoindex[i] = -1;
	}

	log("----\n");
}

void add(char name[NAME_LEN], int mTypeNum, char mTypes[TYPE_CNT][TAG_LEN], int mSection)
{
	log("++++ name=%s ", name);

	// name �����ؼ� NAMEHASH �迭�� �ε����� �Ҵ� �Ѵ�. 
	ll nametemp = getname(name);
	int h = nametemp % NAMEHASH;
	while (nametoindex[h] != -1) {
		h = (h + 1) % NAMEHASH;
	}
	nametoindex[h] = gbc;
	log(" nametoindex[%d]  = %d ", h, nametoindex[h]);

	//mstrcpy(book[gbc].str, name);

	book[gbc].bid = gbc;
	book[gbc].deleted = false;

	book[gbc].name = nametemp;

	F(i, 0, mTypeNum - 1) {
		int tagtemp = gettag(mTypes[i]);
		log("tag = %s, tagtemp = %d", mTypes[i], tagtemp);
		//book[gbc].tag[i] = tagtemp;
		link[glc].bp = &book[gbc];


		//tag ���� �������� hash ���̺��� ����� 
		//������ hash �� ������ ���� ��ũ��� ���´�. 
		h = tagtemp % HASH;

		while (hashhead[h].tag != 0) {
			if (hashhead[h].tag == tagtemp) break;
			h = (h + 1) % HASH;
		}
		hashhead[h].tag = tagtemp;
		log("h = %d, tagtemp = %d", h, tagtemp);

		garolink(&link[glc], hashhead[h].n);
		garolink(&hashhead[h], &link[glc]);

		glc++;

	}

	book[gbc].sid = mSection;

	gbc++;

	log("----\n");
}

int moveType(char mTypes[TAG_LEN], int from, int to)
{
	log("++++ type = %s  from + %d, to =%d", mTypes, from, to);
	int ret = 0;

	//type �� Ư�� tag �� ������ �ִ� ������ �� ���� from �� �ִ� �͸� to �������� �ű�� �ȴ�. 

	//type �� ���� hash �ε����� ���ϰ� �׾ȿ� ��� �׸��� �� ���� �ؾ� �Ѵ�.
	int tagtemp = gettag(mTypes);
	log("tagtemp = %d", tagtemp);

	int h = tagtemp % HASH;

	while (hashhead[h].tag != tagtemp) {
		h = (h + 1) % HASH;
	}

	log("h = %d, tagtemp = %d", h, hashhead[h].tag);

	// �ش� hash ����Ʈ�� �� ó���� �����´� 
	Link * temp = hashhead[h].n;

	// �ش� hash �� �������϶����� �� �����Ѵ�. 
	while (temp != &hashtail[h]) {
		// deleted �� true �̸� ������ ���̴� �Ѿ��. 
		if (temp->bp->deleted != true && temp->bp->sid == from) {

			temp->bp->sid = to;
			log("temp->bid=%d, from =%d, to = %d", temp->bp->bid, from, temp->bp->sid);
			ret++;
		}

		temp = temp->n;
	}

	log("----ret = %d \n", ret);
	return ret;
}

void moveName(char name[NAME_LEN], int mSection)
{
	log("++++ name=%s , sid=%d", name, mSection);
	ll nametemp = getname(name);
	log("nametemp= %lld ", nametemp);

	int h = nametemp % NAMEHASH;
	while (book[nametoindex[h]].name != nametemp) {

		h = (h + 1) % NAMEHASH;
	}
	//nametoindex[h] = nametemp;
	log("nametoindex[%d]  = %d ", h, nametoindex[h]);

	book[nametoindex[h]].sid = mSection;
	log("bid = %d, sid = %d ", nametoindex[h], book[nametoindex[h]].sid);

	log("----\n");
}

void deleteName(char name[NAME_LEN])
{
	log("++++ name=%s ", name);

	ll nametemp = getname(name);
	log("nametemp= %lld ", nametemp);

	int h = nametemp % NAMEHASH;
	while (book[nametoindex[h]].name != nametemp) {
		h = (h + 1) % NAMEHASH;
	}
	//nametoindex[h] = nametemp;
	log("nametoindex[%d]  = %d ", h, nametoindex[h]);

	book[nametoindex[h]].deleted = true;
	log("bid = %d , deleted = %d ", nametoindex[h], book[nametoindex[h]].deleted);

	log("----\n");
}

int countBook(int mTypeNum, char mTypes[TYPE_CNT][TAG_LEN], int mSection)
{
	log("++++ typenum = %d , sid = %d", mTypeNum, mSection);

	int ret = 0;
	bool added[MAX + 1] = { 0, };

	//type tntwk
	F(i, 0, mTypeNum - 1) {

		int tagtemp = gettag(mTypes[i]);
		log("tag = %s, tagtemp = %d", mTypes[i], tagtemp);

		int h = tagtemp % HASH;

		//
		while (hashhead[h].tag != tagtemp) {
			// ���� ���� Ÿ�� (�ؽ�) �� ã�� ���� �ٷ� ������. 
			// �߰����� 0 �� ������ �� Ÿ���� �ؽ��� ���� ���� �ʴ� ���̴�. 
			// �ؽ��� ���ǻ� �߰��� ��� �ִ� ���� ���� �� ����. 
			if (hashhead[h].tag == 0) {
				break;
			}

			h = (h + 1) % HASH;
		}

		// ���� ���� Ÿ�� (�ؽ�) �� ã�� ���� �ٷ� ������. 
		// �߰����� 0 �� ������ �� Ÿ���� �ؽ��� ���� ���� �ʴ� ���̴�. 
		// �ؽ��� ���ǻ� �߰��� ��� �ִ� ���� ���� �� ����. 
		if (hashhead[h].tag == 0) {
			continue;
		}

		log("h = %d, tagtemp = %d", h, hashhead[h].tag);

		// �ش� hash ����Ʈ�� �� ó���� �����´� 
		Link * temp = hashhead[h].n;

		// �ش� hash �� �������϶����� �� �����Ѵ�. 
		while (temp != &hashtail[h]) {
			log("bid  = %d", temp->bp->bid);

			// �̹� ������ book �̶�� �ٽ� ���� �ʿ䰡 ����. 
			if (added[temp->bp->bid] == false) {

				// deleted �� true �̸� ������ ���̴� �Ѿ��.
				if (temp->bp->deleted != true && temp->bp->sid == mSection) {
					log("temp->bid=%d, sid =%d", temp->bp->bid, temp->bp->sid);
					ret++;

					//���� ���� ���ؼ� ���� id �� ���� true �� �صд�. 
					added[temp->bp->bid] = true;
				}
			}

			temp = temp->n;
		}
	}
	log("---- ret = %d\n", ret);
	return ret;
}
