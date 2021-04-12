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

	//여기서 book 안에 tag 가 5개가 올 수 있기 때문에 
	// 아래처럼 한개만 해서는 앞뒤를 링크드리스트를 연결할대 몇번재 tag 에서 연결되는지알 수가 없어 
	// 그냥은 안된다. 
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

// 이름으로 book 을 찾아야 할때가 있으니 이걸로도 해쉬를 만든다 
// 단 이건 리스트로 관리할 필요 없다 ( 이름은 unique 하다고 설정되어 있으니까 
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

	//메인 구조체 인텍스 초기화 
	gbc = 0;
	glc = 0;

	//hash 별 링크드 리스트 초기화 
	F(i, 0, HASH - 1) {
		hashhead[i].n = &hashtail[i];
		//hashtail[i].p = &hashhead[i];
		hashhead[i].tag = 0;
	}

	F(i, 0, NAMEHASH - 1) {
		//book 은 0 번 인덱스부터 시작되니 초기 값은 -1 부터 시작한다. 
		nametoindex[i] = -1;
	}

	log("----\n");
}

void add(char name[NAME_LEN], int mTypeNum, char mTypes[TYPE_CNT][TAG_LEN], int mSection)
{
	log("++++ name=%s ", name);

	// name 에대해서 NAMEHASH 배열의 인덱스를 할당 한다. 
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


		//tag 값을 기준으로 hash 테이블을 만든다 
		//동일한 hash 를 가지는 값을 랑크드로 묶는다. 
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

	//type 즉 특정 tag 를 가지고 있는 도서들 중 섹션 from 에 있는 것만 to 섹션으로 옮기면 된다. 

	//type 에 대한 hash 인덱스를 구하고 그안에 모든 항목을 다 조사 해야 한다.
	int tagtemp = gettag(mTypes);
	log("tagtemp = %d", tagtemp);

	int h = tagtemp % HASH;

	while (hashhead[h].tag != tagtemp) {
		h = (h + 1) % HASH;
	}

	log("h = %d, tagtemp = %d", h, hashhead[h].tag);

	// 해당 hash 리스트의 맨 처음을 가져온다 
	Link * temp = hashhead[h].n;

	// 해당 hash 의 마지막일때까지 다 조사한다. 
	while (temp != &hashtail[h]) {
		// deleted 가 true 이면 지워진 것이니 넘어간다. 
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
			// 만일 없는 타입 (해쉬) 로 찾는 경우는 바로 나간다. 
			// 중간에라도 0 이 나오면 그 타입의 해쉬는 존재 하지 않는 것이다. 
			// 해쉬의 정의상 중간에 비어 있는 값은 나올 수 없다. 
			if (hashhead[h].tag == 0) {
				break;
			}

			h = (h + 1) % HASH;
		}

		// 만일 없는 타입 (해쉬) 로 찾는 경우는 바로 나간다. 
		// 중간에라도 0 이 나오면 그 타입의 해쉬는 존재 하지 않는 것이다. 
		// 해쉬의 정의상 중간에 비어 있는 값은 나올 수 없다. 
		if (hashhead[h].tag == 0) {
			continue;
		}

		log("h = %d, tagtemp = %d", h, hashhead[h].tag);

		// 해당 hash 리스트의 맨 처음을 가져온다 
		Link * temp = hashhead[h].n;

		// 해당 hash 의 마지막일때까지 다 조사한다. 
		while (temp != &hashtail[h]) {
			log("bid  = %d", temp->bp->bid);

			// 이미 더해진 book 이라면 다시 더할 필요가 없다. 
			if (added[temp->bp->bid] == false) {

				// deleted 가 true 이면 지워진 것이니 넘어간다.
				if (temp->bp->deleted != true && temp->bp->sid == mSection) {
					log("temp->bid=%d, sid =%d", temp->bp->bid, temp->bp->sid);
					ret++;

					//다음 값을 위해서 더한 id 는 값을 true 로 해둔다. 
					added[temp->bp->bid] = true;
				}
			}

			temp = temp->n;
		}
	}
	log("---- ret = %d\n", ret);
	return ret;
}
