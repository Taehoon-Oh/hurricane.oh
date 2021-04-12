#define MAX_NODE 400001
#define MAX_TABLE 10007
#define MAX_SIZE 40000
#define MAX_REGION 10
#define MAX_USER 1001
#define MAX_FRIEND 20
#define NULL 0
#define MAX_VAL 1152921504606846976LL
#define V31 2147483647LL
#define alloc() &node_pool[last_alloc++]
long long heap[MAX_REGION][MAX_SIZE];
int heapSize[MAX_REGION];

int heapPush(int region, long long value)
{

	if (heapSize[region] + 1 > MAX_SIZE)
	{
		return 0;
	}
	register long long* h = heap[region];
	register int hs = heapSize[region];
	h[hs] = value;

	register int current = hs;
	while (current > 0 && h[current] < h[(current - 1) >> 1])
	{
		register long long temp = h[(current - 1) >> 1];
		h[(current - 1) >> 1] = h[current];
		h[current] = temp;
		current = (current - 1) >> 1;
	}

	heapSize[region]++;

	return 1;
}

int heapPop(int region, long long* value)
{
	if (heapSize[region] <= 0)
	{
		return 0;
	}
	register long long* h = heap[region];
	*value = h[0];
	heapSize[region]--;
	register int hs = heapSize[region];
	h[0] = h[hs];

	register int current = 0;
	while (current * 2 + 1 < hs)
	{
		register int current2 = current * 2;
		register int child;
		if (current2 + 2 == hs)
		{
			child = current2 + 1;
		}
		else
		{

			child = h[current2 + 1] < h[current2 + 2] ? current2 + 1 : current2 + 2;
		}

		if (h[current] < h[child])
		{
			break;
		}

		register long long temp = h[current];
		h[current] = h[child];
		h[child] = temp;

		current = child;
	}
	return 1;
}

int MM;
int NN;

struct Node {
	int pid;
	int area;
	Node* link;
} node_pool[MAX_NODE];

int last_alloc;

Node table[MAX_TABLE];

int reserved_region[MAX_USER][MAX_REGION];
int frd[MAX_USER][MAX_FRIEND];
int nfriend[MAX_USER];

void init(int N, int M)
{
	last_alloc = 0;
	for (register int i = 0; i < M; i++) {
		heapSize[i] = 0;
	}
	for (register int i = 0; i <= N; i++) {
		for (register int j = 0; j < M; j++) {
			reserved_region[i][j] = 0;
		}
		nfriend[i] = 0;
	}
	for (register int i = 0; i < MAX_TABLE; i++) {
		table[i].link = NULL;
	}
	MM = M;
	NN = N;
}

void befriend(int uid1, int uid2)
{
	frd[uid1][nfriend[uid1]++] = uid2;
	frd[uid2][nfriend[uid2]++] = uid1;
}

void add(int pid, int area, int price)
{
	register long long pp = 0;
	pp += ((price + 0LL) << 32);
	pp += pid;
	heapPush(area - 1, pp);
	register Node* head = &table[pid % MAX_TABLE];
	register Node* node = alloc();
	node->pid = pid;
	node->area = area - 1;
	node->link = head->link;
	head->link = node;
}


void reserve(int uid, int pid)
{
	register Node* node = &table[pid % MAX_TABLE];
	while (node->link) {
		if (node->link->pid == pid) {
			break;
		}
		node = node->link;
	}
	int area = node->link->area;
	//cout << area << endl;
	node->link = node->link->link;
	reserved_region[uid][area]++;
}

int input[MAX_REGION];
int input_id[MAX_REGION];

void quickSort(int first, int last)
{
	int pivot;
	int i;
	int j;
	int temp;

	if (first < last)
	{
		pivot = first;
		i = first;
		j = last;

		while (i < j)
		{
			while (input[i] >= input[pivot] && i < last)
			{
				i++;
			}
			while (input[j] < input[pivot])
			{
				j--;
			}
			if (i < j)
			{
				temp = input[i];
				input[i] = input[j];
				input[j] = temp;
				temp = input_id[i];
				input_id[i] = input_id[j];
				input_id[j] = temp;
			}
		}

		temp = input[pivot];
		input[pivot] = input[j];
		input[j] = temp;
		temp = input_id[pivot];
		input_id[pivot] = input_id[j];
		input_id[j] = temp;

		quickSort(first, j - 1);
		quickSort(j + 1, last);
	}
}


int recommend(int uid)
{
	for (register int i = 0; i < MM; i++) {
		input_id[i] = i;
		input[i] = reserved_region[uid][i];
	}
	register int nf = nfriend[uid];
	for (register int i = 0; i < nf; i++) {
		register int f = frd[uid][i];
		for (register int j = 0; j < MM; j++) {
			input[j] += reserved_region[f][j];
		}
	}

	quickSort(0, MM - 1);

	bool notfound;
	register int pid;
	register Node* node;
	register long long v;
	long long opt = MAX_VAL;
	int prev = 0;
	for (register int i = 0; i < MM; i++) {
		if (input[prev] != input[i] && opt != MAX_VAL) {
			break;
		}
		notfound = true;
		while (notfound) {
			v = heap[input_id[i]][0];
			pid = v & V31;
			node = &table[pid % MAX_TABLE];
			while (node->link) {
				node = node->link;
				if (node->pid == pid) {
					notfound = false;
					if (opt > v) opt = v;
					break;
				}
			}
			if (notfound) {
				if (!heapPop(input_id[i], &v)) break;
			}
		}
		prev = i;
	}
	return opt & V31;
}

//==========================================================

typedef long long int LL;

const int MAX_AREA = 10;
const int MAX_USER = 1000;
const int MAX_FRIENDS = 20;
const int MAX_PACK = 40001;
//const int MAX_PRICE = 1000;
const int MAX_HEAP = 10160;
//const LL MAX_PID = 1000000000;
const LL MAX_PID = 1 << 30;

int nUser, nAreas;

struct User {
	int nfriends;
	int friends[MAX_FRIENDS];
	int cnt_area[MAX_AREA + 1];
};

struct Pack {
	int pid, area;
	//int price;
	int reserved;
	LL d;
};
//int comp(Pack *p, Pack *q) {
//  if (p->price == q->price) {
//      return (p->pid > q->pid);
//  }
//  return (p->price > q->price);
//}

struct Area {
	int num_area;
	int hSize;
	Pack* heap[MAX_HEAP];
	void push(Pack* pack)
	{
		Pack* tmp;
		register int index = hSize;
		heap[hSize++] = pack;

		num_area++;

		//while (index && comp(heap[(index - 1) / 2], heap[index]))
		while (index && heap[(index - 1) / 2]->d > heap[index]->d)
		{
			tmp = heap[(index - 1) / 2];
			heap[(index - 1) / 2] = heap[index];
			heap[index] = tmp;
			index = (index - 1) / 2;
		}
	}
	void pop()
	{
		register int cur = 0, l, r, next;
		heap[0]->pid = heap[0]->reserved = 0;
		heap[0] = heap[--hSize];
		Pack* tmp;
		while (true)
		{
			l = cur * 2 + 1;
			r = cur * 2 + 2;
			if (hSize <= l)  break;

			next = cur;
			//if (comp(heap[next], heap[l]))
			if (heap[next]->d > heap[l]->d)
			{
				next = l;
			}
			//if (r < hSize && comp(heap[next], heap[r]))
			if (r < hSize && heap[next]->d > heap[r]->d)
			{
				next = r;
			}
			if (next == cur)
			{
				break;
			}
			tmp = heap[next];
			heap[next] = heap[cur];
			heap[cur] = tmp;
			cur = next;
		}
	}

	void check_reserved()
	{
		while (hSize > 0 && heap[0]->reserved)    pop();
	}
};

User buf_users[MAX_USER + 1];
Pack buf_packs[MAX_PACK];
Area buf_areas[MAX_AREA + 1];

int cnt_areas[MAX_AREA + 1];

int comp_all(Pack *p, Pack *q) {

	if (cnt_areas[p->area] == cnt_areas[q->area]) {
		//if (p->price == q->price) {
		//  return (p->pid > q->pid);
		//}
		//return (p->price > q->price);
		return (p->d > q->d);
	}
	return cnt_areas[p->area] < cnt_areas[q->area];
}

Pack* genPack(int pid)
{
	register int index = pid % MAX_PACK;
	while (buf_packs[index].pid > 0)
	{
		index = (index + 1) % MAX_PACK;
	}
	return &buf_packs[index];
}

Pack* getPack(int pid)
{
	register int index = pid % MAX_PACK;
	while (buf_packs[index].pid != pid)
	{
		index = (index + 1) % MAX_PACK;
	}
	return &buf_packs[index];
}

void init(int N, int M)
{
	//printf("\n === INIT(%d) ===\n", ++tcc);
	register int n, m;

	nUser = N + 1;
	nAreas = M + 1;
	for (n = 1; n < nAreas; n++) {
		buf_areas[n].hSize = buf_areas[n].num_area = 0;
	}
	for (n = 1; n < nUser; n++) {
		for (m = 1; m < nAreas; m++) buf_users[n].cnt_area[m] = 0;
		buf_users[n].nfriends = buf_packs[n].pid = 0;
	}
	for (; n < MAX_PACK; n++) {
		buf_packs[n].pid = 0;
	}
}

void befriend(int uid1, int uid2)
{
	//printf("befriend(%d, %d)\n", uid1, uid2);
	buf_users[uid1].friends[buf_users[uid1].nfriends++] = uid2;
	buf_users[uid2].friends[buf_users[uid2].nfriends++] = uid1;
}

void add(int pid, int area, int price)
{
	//printf("add(pid = %d, area = %d, price = %d)\n", pid, area, price);
	register Pack* p = genPack(pid);
	p->pid = pid;
	p->area = area;
	//p->price = price;
	p->reserved = false;
	p->d = ((LL)price << 30 | pid);
	//p->d = (price * MAX_PID + pid - 1);
	buf_areas[area].push(p);
}

void reserve(int uid, int pid)
{
	//printf("reserve(uid = %d, pid = %d)\n", uid, pid);
	register Pack* p = getPack(pid);

	p->reserved = true;
	buf_users[uid].cnt_area[p->area]++;
	buf_areas[p->area].num_area--;
}

int recommend(int uid)
{
	//printf("recommend(uid = %d)\n", uid);
	register Pack *p, *q;
	register int n, m, num_friends;

	for (n = 1; n < nAreas; ++n)
	{
		cnt_areas[n] = buf_users[uid].cnt_area[n];
		buf_areas[n].check_reserved();
	}
	num_friends = buf_users[uid].nfriends;
	for (m = 0; m < num_friends; ++m)
	{
		register User* f = &buf_users[buf_users[uid].friends[m]];
		for (n = 1; n < nAreas; ++n)
		{
			cnt_areas[n] += f->cnt_area[n];
		}
	}

	for (n = 1; n < nAreas; ++n) if (buf_areas[n].hSize > 0)  break;
	p = buf_areas[n].heap[0];

	for (m = n + 1; m < nAreas; ++m)
	{
		if (buf_areas[m].hSize > 0)
		{
			q = buf_areas[m].heap[0];
			if (comp_all(p, q)) p = q;
		}
	}
	return p->pid;
}

