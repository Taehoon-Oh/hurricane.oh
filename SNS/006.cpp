#define EE 0



#if EE
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a "\n",__FUNCTION__,__LINE__,##__VA_ARGS__)

#else
#define log(a,...)

#endif 

#define F(a,b,c) for(register int a=b;a<=c;a++)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

template <typename T>
void swap(T & a, T & b) { T t = a; a = b; b = t; }

template <typename T>
void garolink(T & l, T & r) { l->n = r; r->p = l; }

#define MU      1000
#define MAX     50000
#define ML      550000
#define HASH    100007  // (MAX*2 +7) 

// The below commented functions are for your reference. If you want
// to use it, uncomment these functions.
typedef unsigned long long ll; 

class Msg {
public:
	int mid; 
	int score; // 시간을 절대 숫자로 환산하고 id  를 뺀값 
	//23:59:59  
	//59   : 9 보다 큰 2의 배수는 16 2^4 , 5보다 큰 2의 배수 8 = 2^3; 
}msg[MAX +1]; // mid 가 큰수기 때문에 hash 로 찾아가야 한다. 
int gmc = 0; 

class Link {
public: 
	Msg * mp; 
	Link *n;
	Link *p;
}link[ML+1];
int glc = 0; 

ll hash[HASH]; 
Link hashhead[HASH], hashtail[HASH]; 
Link userhead[MU ], usertail[MU ]; 

int fmap[MU + 1][MU + 1]; // 친구 관계를 나타낸다.


void init()
{
	log("+++");

	gmc = 0; 
	F(i, 0, HASH - 1) {
		
		hashhead[i].n = &hashtail[i]; 
		//hashtail[i].p = &hashhead[i]; 
	}
	F(i, 0, MU-1) {
		fmap[i][0] = 1;
		fmap[i][1] = i;
		userhead[i].n = &usertail[i];
		//usertail[i].p = &userhead[i];
	}
	glc = 0; 
	log("---");
}

void createMessage(int mid, int uid, char data[])
{
	log("+++ mid = %d , uid = %d ", mid, uid);
	int score = (((data[0] - '0') << 17 | (data[1] - '0') << 14 | (data[3] - '0') << 11 | (data[4] - '0') << 7 | (data[6] - '0') << 4 | (data[7] - '0'))<<10) -uid; 
	log("score= %d ", score);
   
	msg[gmc] = { mid, score }; 
	Msg * mp = &msg[gmc++]; 
	// msg 가 MAX 만큼 생성될 것이고 순차적으로 mid 를 넣어 놓고 mp를 확보한다. 
	char * e=&data[10]; 
	ll t = 0; 
	Link * lp = 0; 

	//  hash 가 MAX *10 개 생성될 수 있고 mp 와 링크로 엮어 준다. 
	while (1) {

		if (*e == '#') e++; 
		else if (*e == ' ' || *e =='\0') {
			// hash 계산하고 
			int h = t % HASH; 
			for (;;h=(h+1)%HASH) {
				if (hash[h] != 0 && hash[h] != t)continue; 
				if (hash[h] == 0) hash[h] = t; break;
			}
			log("hash[%d]=%lld", h, t); 
			lp = &link[glc++];
			lp->mp = mp;
			Link * temp = &hashhead[h]; 
			for (; temp->n != &hashtail[h]; temp = temp->n) {
				if (temp->n->mp && temp->n->mp->score < mp->score) break;
			}

			lp->n = temp->n;
			temp->n = lp;

			
			//temp->p = lp;

			

			if (*e == '\0') break;
			else { t = 0, e++; }
		}
		else {
			t = (t << 5) + *e - 96; 
			e++; 
		}
	}	

	// uid 에 대해서 mp 와 링크로 엮어 준다. 
	lp = &link[glc++]; 
	lp->mp = mp; 
	lp->n = userhead[uid].n; 
	//userhead[uid].n->p = lp; 

	//lp->p = &userhead[uid];
	userhead[uid].n = lp;

	log("---");
}

void followUser(int u1, int u2)
{
	log("+++");
	fmap[u1][++fmap[u1][0]] = u2; 


	log("---fmap[%d][0]= %d , %d added", u1, fmap[u1][0], u2);
}

int searchByHashtag(char name[], int ids[])
{
	log("+++");

	char * e = &name[1]; 
	ll t = 0; 
	int h = 0; 

	while (1) {

		if (*e == '\0') {
			// hash 계산하고 
			h = t % HASH;
			for (;; h = (h + 1) % HASH) {
				if (hash[h] != 0 && hash[h] != t)continue;
				if (hash[h] == 0) return 0; 
				break;
			}
			log("hash[%d]=%lld", h, t);	
			break;
		}
		else {
			t = (t << 5) + *e - 96;
			e++;
		}
	}
	t = 0; 

	for (Link * lp = hashhead[h].n ; lp != &hashtail[h] && t < 5; lp=lp->n, t ++) {
		ids[t] = lp->mp->mid; 
	}

	// tag 의 hash 를 구하고 거기서 mp 를 통해서 mid 를 구하면된다. 

	log("---");
	return t;
}

int getMessages(int uid, int ids[])
{
	Msg * list[MAX]; 
	int total = 0; 
	log("+++");
	F(i, 1, fmap[uid][0]) {
		int fid = fmap[uid][i]; 
		int ret = 0; 
		for (Link * lp = userhead[fid].n; lp != &usertail[fid] && ret < 5; lp = lp->n, ret++) {
			list[total++] = lp->mp; 
		}
	}

	F(i, 0, Min(total-1, 4) ) {
		F(j, i + 1, total-1) {
			if (list[i]->score < list[j]->score) swap(list[i], list[j]); 
		}
		ids[i] = list[i]->mid; 
	}

	log("---");

	return Min(total,5) ;
}