#define MAXL    5
#define MAXF    10
#define MAXN 10000
#define MAXC 50000
#define r_ register

struct Node {
	int id;
	Node* next;
}n[MAXC * MAXF * 2];

int n_idx;
Node* n_alloc(int id) {
	r_ Node* temp = &n[n_idx++];
	temp->id = id;
	temp->next = 0;
	return temp;
}

struct User {
	Node* add;
	Node* del;
}user[MAXN + 1];


int NN;
int count[MAXN + 1];
int flag[MAXN + 1];

void init(int N)
{
	NN = N;
	n_idx = 0;

	for (r_ int i = 1; i <= N; i++) {
		user[i].add = 0;
		user[i].del = 0;
	}
}

void add(int id, int F, int ids[MAXF])
{
	for (r_ int i = 0; i < F; i++) {
		r_ Node* temp = n_alloc(ids[i]);
		temp->next = user[id].add;
		user[id].add = temp;

		temp = n_alloc(id);
		temp->next = user[ids[i]].add;
		user[ids[i]].add = temp;
	}
}

void del(int id1, int id2)
{
	r_ Node* temp = n_alloc(id2);
	temp->next = user[id1].del;
	user[id1].del = temp;

	temp = n_alloc(id1);
	temp->next = user[id2].del;
	user[id2].del = temp;
}

int recommend(int id, int list[MAXL])
{
	for (r_ int i = 1; i <= NN; i++) {
		flag[i] = 0;
		count[i] = 0;
	}

	count[id]--;
	r_ Node* p;
	for (p = user[id].add; p; p = p->next) {
		count[p->id]--;
	}
	for (p = user[id].del; p; p = p->next) {
		count[p->id]++;
	}
	for (p = user[id].add; p; p = p->next) {
		if (count[p->id] != -1) {
			continue;
		}

		r_ Node* ff;
		for (ff = user[p->id].add; ff; ff = ff->next) {
			if (count[ff->id] >= 0) count[ff->id]++;
		}
		for (ff = user[p->id].del; ff; ff = ff->next) {
			if (count[ff->id] >= 0) count[ff->id]--;
		}
		count[p->id]--;
	}

	r_ int top = 0;
	r_ int result = 0;

	for (r_ int i = 0; i < 5; i++) {
		top = 0;

		for (r_ int j = 1; j <= NN; j++) {
			if (count[top] < count[j]) {
				top = j;
			}
		}

		if (top == 0)
			break;

		list[i] = top;
		count[top] = 0;
		result++;
	}

	return result;
}
