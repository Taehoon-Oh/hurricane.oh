#define EE 1

#if EE 
#include <stdio.h>
#define log(a,...)	printf("%10s[%03d]: " a "\n", __FUNCTION__, __LINE__,##__VA_ARGS__)

#else

#define log(a,...)

#endif 

#define F(a,b,c)	for ( register int a = b ; a<=c; a++)
#define Max(a,b)	(((a)>(b))?(a):(b))
#define Min(a,b)	(((a)<(b))?(a):(b))

template < typename T > 
void swap(T & a, T & b) { T t = a; a = b; b = t; }

#define MU (10000)
#define MS (10)
#define MC (1000)

class Map {
public: 
	int id;
	int dist;

	int operator < ( Map pre)	{
		if (dist == pre.dist) return id > pre.id; 
		else return dist > pre.dist; 	
	}
};



class User {
public:
	int uid;
	int sid;
	int x; 
	int far; 
	Map map[MS];
	User* n; 
	User* p; 


	int operator <  (User pre) {
		if (far == pre.far) return uid < pre.uid;
		else return far > pre.far; 
	}
}user[MU];

int guc = 0; 
int umap[10000];

User Head; 
User Tail; 

User* newUser(int uid, int x) {
	umap[uid] = guc; 
	User* up = &user[guc++]; 
	up->uid = uid; 
	up->x = x; 
	up->far = -1; 
	up->sid = -1; 
	return up; 
}



class Server {
public: 
	int sid;
	int x; 
	int cnum;
	int list[MC]; 
}server[MS];


int L, N, C; 


void init(int l, int n, int c, int x[MS])
{
	log("+++ L=%d, N= %d, C= %d ", l, n, c); 
	F(i, 0, 9999) {
		umap[i] = -1; 
	}
	guc = 0; 


	Head.n = &Tail; 
	Head.p = Tail.n = 0; 
	Tail.p = &Head; 

	L = l, N = n, C = c;

	// 서버등 위치 저장 
	F(i, 0, N - 1) {
		log("server x[%d] = %d", i, x[i]); 
		server[i].sid = i; 
		server[i].x = x[i]; 
	} 


	log("---");

}

int getDist(int a, int b) {
	int ret = 0; 

	ret = Min(Max(a, b) - Min(a, b), Min(a, b) + L - Max(a, b)); 

	log("dist = %d", ret); 
	return ret; 

}

int Assign(int uid) {
	int ret = 0; 

	User* temp = Head.n; 

	F(i, 0, N - 1) {
		server[i].cnum = 0; 		
	}

	while (temp != &Tail) {

		F(i, 0, N - 1) {
			int sid = temp->map[i].id; 
			int& cnum = server[sid].cnum; 
			
			if (cnum< C) {				
				server[sid].list[cnum++] = temp->uid;
				temp->sid = sid;			

				log("uid = %d sid = %d, cnum = %d", temp->uid, sid, cnum);

				if (temp->uid == uid) ret = sid; 
				break; 
			}
			else {
				continue; 
			}

		}
		temp = temp->n;
	}

	return ret;
}


int add_user(int uid, int x)
{
	log("+++ uid = %d, x = %d", uid, x );

	User* up = newUser(uid, x);

	// 해당 유저와 각 서버들 간의 거리를 구한다. 
	//
	int max = -1; 
	F(i, 0, N - 1) {
		up->map[i].id = i; 
		up->map[i].dist = getDist(x, server[i].x); 
		max= Max(max, up->map[i].dist); 
	}

	F(i, 0, N - 2) {
		F(j, i + 1, N - 1, ) {
			if (up->map[i] < up->map[j]) swap(up->map[i], up->map[j]); 
		}
	}
	up->far = max; 
	

	// 최대 거리 구하고, 가까운 서버 순으로 정렬한다. 

	//최대 거리 기준으로 linked list 에 넣어준다. 
	User* temp = Head.n ; 
	if (temp == &Tail) {
		up->n = Head.n;
		up->p = &Head;
		Head.n->p = up;
		Head.n = up;
	}
	else {
		while (temp != &Tail) {
			if (*up < *temp) {

				//  temp -> p   ==== up ====== temp 
								
				break; 
			}
			temp = temp->n;			
		}
		temp->p->n = up;
		up->n = temp;

		up->p = temp->p;
		temp->p = up;
	}

	int ret = 0; 
	ret = Assign(uid); 

	log("---ret %d	\n", ret);
	return ret;
}

int remove_user(int uid)
{
	log("+++ uid = %d ", uid);

	User* up = &user[uid]; 

	User* left = up->p; 
	User* right = up->n; 
	left->n = right; 
	right->p = left; 

	Assign(uid); 

	int ret = up->sid; 
	log("---ret = %d \n",ret);
	return ret;
}

int get_users(int sid)
{
	log("+++ sid = %d ", sid );
	int ret = 0; 

	ret = server[sid].cnum; 


	log("---ret = %d \n",ret );
	return ret;
}