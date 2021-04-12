#define EE 0

#include <stdio.h>

#if EE

#define log(a,...)	printf("%10s[%03d]: " a "\n", __FUNCTION__, __LINE__,##__VA_ARGS__)


#else 
#define log(a,...)

#endif 

#define F(a,b,c)	for( register int a=b;a<=c;a++)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

#define Null 0 

template <typename  T>
void swap(T & a, T & b) { T t = a; a = b; b = t; }

#define MU (10000)
#define MS (10)
#define MC (1000)

using namespace std; 

class Smap {
public:
	int sid; 
	int dist; 

	int operator < (Smap r) {
		if (dist == r.dist)return sid > r.sid; 
		else return dist > r.dist; 
	}
};


class User {
	public: 
	int uid; 
	int x; 
	int setsid; 
	int max; 
	Smap smap[MS]; 

	bool operator > (User r) {
		if (max == r.max) return uid <r.uid; 
		else return max > r.max; 
	}

	User* n; 
	User* p; 

}user[MU];
int guc; 

int L, S, C;

int umap[MU]; // uid 가 몇번재 user 를 사용했는지 바로 접근하도록 그래야 remove 할때 linked 바로 삭제 가능. 

User Head, Tail; 

User* newUser(int x, int uid) {
	User* up = &user[guc++]; 
	up->x = x; 
	up->uid = uid;
	up->setsid = -1; 
	up->max = -1; 
	F(i, 0, S-1)up->smap[i] = { 0, }; 

	return up; 
}

class Server {
public: 
	int x; 
	int cnum; 
	int clients[MC]; 
}server[MS];

//struct  comp {
//	bool operator () (Smap a, Smap b) {
//		if (a.dist == b.dist) return a.sid > b.sid;
//		else return a.dist > b.dist; 
//	}
//};

//priority_queue < Smap, vector <Smap>, comp> pq; 

void initServer() {
	F(i, 0, S - 1) {
		server[i].cnum = 0;
	}
}

void init(int l, int s, int c, int x[MS])
{
	log("+++ L=%d, S=%d, C=%d", l, s, c ); 
	L = l, S = s, C = c; 
	guc = 0; 

	initServer(); 

	F(i, 0, S - 1) {
		server[i].x = x[i]; 
		log("x[%d]=%d", i, x[i]); 
	}


	Head.n = &Tail; 
	Head.p = Tail.n = Null; 
	Tail.p = &Head; 

	Head.uid = -1; 
	Head.max = 20000; 

	Tail.uid = 10000; 
	Tail.max = -1; 


	log("---" "\n"); 

}
int getDist(int x1, int x2) {
	int ret = 0; 

	ret = Min(Max(x1, x2) - Min(x1, x2), Min(x1, x2) + L- Max(x1, x2)); 

	//log("L= %d, x1=%d, x2 = %d ret = %d ", L, x1, x2, ret); 
	return ret; 
}

void addLink(User* l, User* r) {

	log(" uid(%d),dist(%d) -> uid(%d),dist(%d)", r->p->uid, r->p->max, r->uid, r->max); 
	l->n = r; 
	l->p = r->p; 
	r->p->n = l; 
	r->p = l; 

	log(" uid(%d),dist(%d) -> uid(%d),dist(%d)-> uid(%d),dist(%d)", l->p->uid, l->p->max, l->uid, l->max, r->uid, r->max);


}
void assign() {

	initServer(); // 서버에 할당된 사용자 숫자 초기화 시키고 

	// 새로 우선 순서대로 다시 할당. 
	User* temp = Head.n;

	while (temp != &Tail)
	{
		F(i, 0, S - 1) {
			int sid = temp->smap[i].sid; 
			int dist = temp->smap[i].dist; 
			int cnum = server[sid].cnum; 


			if (cnum < C )
			{
				temp->setsid = sid; 
				server[sid].cnum++; 

				break; 
			}
			//printf("\n");

		}
		temp = temp->n; 
	}


}

int add_user(int uid, int x)
{
	User* up = newUser(x, uid);
	umap[guc - 1] = uid;

	log("+++ guc = %d uid =%d, x=%d  ", guc -1,  uid, x);
	int max = -1; 
	F(i, 0, S - 1) {
		up->smap[i].sid = i; 
		up->smap[i].dist = getDist(x, server[i].x);		
		max = Max (max, up->smap[i].dist); 
	}
	up->max = max; 
	//log("max = %d ", max);

	F(i, 0, S - 2) {
		F(j, i + 1, S - 1) {
			if (up->smap[i] < up->smap[j]) swap(up->smap[i], up->smap[j]); 
		}		
	}

	User* temp = Head.n;

	while (temp !=&Tail)
	{
		if (*up > *temp )
		{
			break; 
		}
		else
		{
			temp = temp->n; 
		}
	}
	// temp 앞에 up  를 넣는다. 
	addLink(up, temp); 

	assign(); 


	log("---uid %d -> sid %d" "\n", uid, up->setsid);
	return up->setsid;
}

int remove_user(int uid)
{
	int ret = 0; 
	log("+++ uid =%d", uid);
	User* up = &user[umap[uid]]; 
	ret = up->setsid; 
	log("+++ up->uid  =%d", up->uid);	

	log(" uid(%d) -> uid(%d)-> uid(%d)", up->p->uid, up->uid, up->n->uid);
	User* l, * r; 
	l = up->p; 
	r = up->n; 
	l->n = r; 
	r->p = l; 
	log(" uid(%d) -> uid(%d)", up->p->uid, up->p->n->uid);		

	assign(); 
	log("---ret =  %d " "\n", ret );
	return ret;
}

int get_users(int sid)
{
	int ret = 0; 
	log("+++ sid=%d", sid);

	ret = server[sid].cnum; 

	log("---ret =  %d " "\n", ret);
	return ret; 
}



